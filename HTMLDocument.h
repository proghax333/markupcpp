
#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "HTMLElementTypes.h"
#include "HTMLElement.h"

using namespace std;

class HTMLDocument {
	public:
		HTMLElement* root;
		HTMLDocument() = default;
		explicit HTMLDocument(const HTMLElement& root) :
			root((HTMLElement*)&root) { }
		explicit HTMLDocument(HTMLElement* root) :
			root(root) { }
		
		void setRoot(HTMLElement* root) {
			this->root = root;
		}
		void setRoot(const HTMLElement& root) {
			this->root = (HTMLElement*)&root;
		}
		HTMLElement* getRoot() {
			return root;
		}
		void display() {
			if(root != NULL) {
				root->displayElement();
			} else {
				cout << "EMPTY DOCUMENT!";
			}
		}
		HTMLElement* getElementById(const string& id) {
			if(root == NULL) {
				return NULL;
			}
			return root->getElementById(id);
		}
		vector<HTMLElement*> getElementsByClassName(const string& className) {
			vector<HTMLElement*> result;
			root->getElementsByClassName(result, className);
			return result;
		}
};
