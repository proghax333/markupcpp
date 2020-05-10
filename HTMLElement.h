
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include "HTMLElementTypes.h"

class HTMLElement {
	public:
		vector<HTMLElement*> children;
		HTMLElement* parent = NULL;
		Attributes attributes;
		string type, text;
		
		HTMLElementType elementType;
		
		HTMLElement(const string& type, HTMLElementType elementType = NORMAL) :
			type(type),
			elementType(elementType) { }
		HTMLElement(const string& type, const Attributes& attributes, HTMLElementType elementType = NORMAL) :
			type(type),
			attributes(attributes),
			elementType(elementType) { }
		
		void addElement(HTMLElement& element) {
			element.setParent(this);
			children.push_back((HTMLElement*)&element);
		}
		void removeElement(HTMLElement& element) {
			int count = 0;
			for(HTMLElement* child : children) {
				if(child == &element) {
					children.erase(children.begin() + count);
					break;
				}
				++count;
			}
		}
		void addNode(HTMLElement& element) {
			if(element.parent == NULL) {
				children.push_back(&element);
			} else {
				element.parent->removeElement(element);
				children.push_back(&element);
			}
		}
		string& getText() {
			return text;
		}
		void setText(const string& text) {
			this->text = text;
		}
		HTMLElement* getParent() {
			return parent;
		}
		void setParent(HTMLElement* parent) {
			this->parent = parent;
		}
		void displayElement() {
			displayRecursive(this, 0);
		}
		bool attributeExists(const string& name) {
			return attributes.count(name) > 0;
		}
		vector<string> getClasses() {
			vector<string> result;
			if(attributeExists("class")) {
				return getClasses(attributes["class"]);
			}
			return result;
		}
		vector<string> getClasses(const string& className) {
			vector<string> classes;
			int size = className.size();
			int prev = 0;
			for(int i = 0; i < size; ++i) {
				if(className[i] == ' ') {
					string s = className.substr(prev, i - prev);
					classes.push_back(s);
					prev = i + 1;
				} else if(i == size - 1) {
					string s = className.substr(prev, size);
					classes.push_back(s);
				}
			}
			return classes;
		}
		HTMLElement* getElementById(const string& id) {
			return searchIdRecursive(this, id);
		}
		vector<HTMLElement*> getElementsByClassName(const string& className) {
			vector<HTMLElement*> results;
			searchClassRecursive(results, this, className, getClasses(className));	
			return results;
		}
		void getElementsByClassName(vector<HTMLElement*>& results, const string& className) {
			searchClassRecursive(results, this, className, getClasses(className));	
		}
	protected:
		HTMLElement* searchIdRecursive(HTMLElement* e, const string& id) {
			if(e->attributeExists("class")) {
				if(e->attributes["id"] == id) {
					return e;
				}
			}
			HTMLElement* result;
			for(HTMLElement* child : e->children) {
				result = searchIdRecursive(child, id);
				if(result != NULL) {
					return result;
				}
			}
			return NULL;
		}
		void searchClassRecursive(vector<HTMLElement*>& results, HTMLElement* e, const string& className, const vector<string>& parsedClassNames) {
			if(e != NULL) {
				if(className.empty() || parsedClassNames.size() == 0)
					return;
				
				bool found = false, valid = true;
				if(e->attributes.count("class") > 0) {
					if(e->attributes["class"] == className) {
						results.push_back(e);
					} else {
						vector<string>&& classes = e->getClasses();
						for(auto& parsedClassName : parsedClassNames) {
							for(auto& c : classes) {
								if(c == parsedClassName) {
									found = true;
									break;
								}
							}
							if(found == false) {
								valid = false;
								break;
							}
							found = false;
						}
						if(valid) {
							results.push_back(e);
						}
					}
				}
				for(HTMLElement* child : e->children) {
					searchClassRecursive(results, child, className, parsedClassNames);
				}
			}
		}
		void printSpaces(int depth) {
			for(int i = 0; i < depth; ++i) {
				cout << "  ";
			}
		}
		void printTagStart(HTMLElement* e, int depth) {
			printSpaces(depth);
			cout << "<" << e->type;
			for(const Attribute& attribute : e->attributes) {
				cout << ' ' << attribute.first << "=\"" << attribute.second << "\"";
			}
			if(e->elementType == NORMAL)
				cout << ">";
		}
		void printChildren(HTMLElement* e, int depth) {
			for(HTMLElement* child : e->children) {
				cout << endl;
				displayRecursive(child, depth + 1);
			}
		}
		void printText(HTMLElement* e, int depth) {
			if(!e->text.empty()) {
				cout << endl;
				printSpaces(depth + 1);
				cout << e->text;
			}
		}
		void printTagEnd(HTMLElement* e, int depth) {
			if(e->elementType == NORMAL) {
				cout << endl;
				printSpaces(depth);
				cout << "</" << e->type << ">";
			} else {
				cout << " />";
			}
		}
		void displayRecursive(HTMLElement* e, int depth) {
			if(e != NULL) {
				printTagStart(e, depth);
				if(e->elementType == NORMAL) {
					printChildren(e, depth);
					printText(e, depth);
				}
				printTagEnd(e, depth);
			}
		}
		void validate() {
			for(HTMLElement* child : children) {
				if(child != NULL) {
					child->parent = this;
				}
			}
		}
};
