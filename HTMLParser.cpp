
#include <iostream>
#include <vector>
#include <unordered_map>

#include "HTMLElementTypes.h"
#include "HTMLElement.h"
#include "HTMLDocument.h"

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
	
	body.addElement(heading);
	body.addElement(heading2);
	body.text = "This is just the body!";
	
	HTMLElement input("input", NOCLOSE);
	input.attributes["name"] = "username";
	input.attributes["type"] = "text";
	input.attributes["class"] = "input input-text";
	input.attributes["placeholder"] = "Enter Your Username";
	input.attributes["id"] = "form-username";
	
	body.addElement(input);
	
	root.addElement(head);
	root.addElement(body);
	
	document.root = &root;
	
	cout << "*** ORIGINAL DOCUMENT ***" << endl << endl;
	document.display();
	cout << endl << endl;
	
	
	/* Changing Branches */
	head.addNode(heading2);
	cout << endl << endl << "BRANCHES CHANGED!" << endl << endl;
	document.display();
	
	return 0;
}

