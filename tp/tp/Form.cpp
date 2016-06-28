#include "stdafx.h"
#include <string.h>
#include "InputText.h"
#include "InputCheckBox.h"
#include "Select.h"
#include "RadioButtonList.h"
#include "InputButton.h"
#include<iostream>
#include "Form.h"

using namespace std;

using std::string;
using UndavInputTextBox::TextBox;
using UndavInputCheckBox::CheckBox;
using UndavSelect::Select;
using UndavRadioButtonList::RadioButtonList;
using UndavInputButton::Button;

enum  elementsHtmlType {
	tInput,
	tSelect,
	tRadioButtonList,
	tInputButton,
	tSubmitButton
};

struct Elements {
	elementsHtmlType Type;
	void* elements;
	string name;
	Elements* next;
};

struct UndavForm::Form {
	OnFormSubmitted submitFunction; //Contiene la funcion que se ejecuta cunado se hace el submit del form.
	bool hasSubmitButton;
	Elements* elements;
};

typedef void(*OnFormSubmitted)(UndavForm::Form* form);

UndavForm::Form * UndavForm::CreateForm(OnFormSubmitted onSubmitted)
{
	Form* form = new Form;
	form->elements = new Elements;
	form->elements->elements = NULL;
	form->elements->name = "";
	form->elements->next = NULL;
	form->submitFunction = onSubmitted;
	form->hasSubmitButton = false;
	return form;
}

Elements* CreateElement(string fieldName, elementsHtmlType elementType, void* element) {
	Elements* elements = new Elements;
	elements->name = fieldName;
	elements->Type = elementType;
	elements->elements = element;
	elements->next = NULL;
	return elements;
}

void UndavForm::AddField(Form * form, string fieldName, Input * input)
{
	if (!form->hasSubmitButton) {
		Elements* auxElement = form->elements;
		Elements* last = form->elements;

		if (form->elements->name != "") {
			auxElement = CreateElement(fieldName, tInput, input);
			while (last->next != NULL)
				last = last->next;
			last->next = auxElement;
		}
		else {
			form->elements = CreateElement(fieldName, tInput, input);
		}
	}
}

void UndavForm::AddField(Form * form, string fieldName, Select * select)
{
	if (!form->hasSubmitButton) {
		Elements* auxElement = form->elements;
		auxElement = CreateElement(fieldName, tSelect, select);

		//Ojo con esto!
		if (form->elements != NULL) {
			while (auxElement->next != NULL) {
				auxElement = auxElement->next;
			}
		}
		//Ojo con esto!
	}
}

void UndavForm::AddField(Form * form, string fieldName, RadioButtonList * radioButtons)
{
	if (!form->hasSubmitButton) {
		Elements* auxElement = form->elements;
		auxElement = CreateElement(fieldName, tRadioButtonList, radioButtons);

		//Ojo con esto!
		if (form->elements != NULL) {
			while (auxElement->next != NULL) {
				auxElement = auxElement->next;
			}
		}
		//Ojo con esto!
	}
}

void UndavForm::SetSubmitButton(Form * form, Button * submitButton)
{
	Elements* auxElement = form->elements;

	//Ojo con esto!
	if (form->elements != NULL) {
		while (auxElement->next != NULL && auxElement->Type == tSubmitButton) {
			auxElement = auxElement->next;
		}
	}
	//Ojo con esto!

	if (form->hasSubmitButton)
		auxElement->elements = submitButton;
	else
		auxElement->elements = CreateElement("NULL", tSubmitButton, submitButton);

}

void UndavForm::Submit(Form * form)
{
	if (form->hasSubmitButton)
		form->submitFunction(form);

}

string CreateLabel(string name) {
	string htmlText;
	htmlText = "<label>" + name + "</label>\n</br>\n";
	return htmlText;
}

string CreateHtmlRadioButtonList(UndavRadioButtonList::RadioButtonList* radioButtons) {
	string htmlText = "";
	int index = 0;
	int countElements = UndavRadioButtonList::Count(radioButtons);
	while (index <= countElements) {
		htmlText = htmlText + UndavInput::GetHtmlText(UndavInputRadio::GetInputElement(UndavRadioButtonList::GetRadioButton(radioButtons, index)));
		if (index % 3 == 0) {
			htmlText = htmlText + "\n";
		}
		index++;
	}
	htmlText = htmlText + "</br>\n";

	return htmlText;
}

string UndavForm::GetHtmlText(Form * form)
{
	Elements* auxElement = form->elements;
	//Elements* saveInputButton;
	string html = "<form>\n";
	while (auxElement != NULL) {
		switch (auxElement->Type) {
		case tInput:
			if (auxElement->name != "NULL") {

				html = html + CreateLabel(auxElement->name);
			}
			html = html + UndavInput::GetHtmlText((UndavInput::Input*)auxElement->elements);
			break;
		case tRadioButtonList:
			html = html + CreateLabel(auxElement->name) + CreateHtmlRadioButtonList((UndavRadioButtonList::RadioButtonList*)auxElement->elements);
			break;
		case tSelect:
			html = html + CreateLabel(auxElement->name) + UndavSelect::GetHtmlText((UndavSelect::Select*)auxElement->elements) + "</br>\n";
			break;
		case tInputButton:
			// saveInputButton = auxElement;
			break;
		default:
			break;
		}
		auxElement = auxElement->next;
	}

	return html;
}

string UndavForm::Serialize(Form * form)
{
	return string();
}

void UndavForm::DestroyForm(Form * form)
{
}
