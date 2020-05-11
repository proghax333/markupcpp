
#include <iostream>
#include <vector>
#include <unordered_map>

#include "HTMLElementTypes.h"
#include "HTMLElement.h"
#include "HTMLDocument.h"
#include "HTMLStyles.h"

using namespace std;

int main() {
	HTMLDocument document;
	
	HTMLElement root("html");
	HTMLElement head("head");
	HTMLElement body("body");
	HTMLElement heading ("h1", {
		{ "class", "heading heading-main"},
		{ "id", "main-heading" }
	});
	heading.text = "This is just a sample heading tag!";
	HTMLElement heading2 ("h2", {
		{ "class", "heading heading-sub"},
		{ "id", "sub-heading" }
	});
	heading2.text = "This is just a sample heading tag!";
	
	HTMLElement input("input", NOCLOSE);
	input.attributes["name"] = "username";
	input.attributes["type"] = "text";
	input.attributes["class"] = "input input-text";
	input.attributes["placeholder"] = "Enter Your Username";
	input.attributes["id"] = "form-username";
	
	HTMLElement mainContainer("div");
	mainContainer.attributes["class"] = "main-container";
	
	HTMLElement paragraph("p");
	paragraph.text = "This is just a sample paragraph!";
	
	HTMLElement span("span");
	span.text = "This is just another span!";
	
	mainContainer.addElement(span);
	mainContainer.addElement(paragraph);
	
	body.addElement(input);
	body.addElement(heading);
	body.addElement(heading2);
	body.addElement(mainContainer);
	body.text = "This is just the body!";
	
	root.addElement(head);
	root.addElement(body);
	
	document.root = &root;
	
	document.display();
	cout << endl << endl;
	
	cout << "POSITION OF paragraph: " << endl;
	cout << endl << heading2.calculatePosition();
	
	return 0;
}

