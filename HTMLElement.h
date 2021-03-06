
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

#define INDENT "  ";

#include "HTMLElementTypes.h"

using namespace std;

vector<string> excludedTypes = { "style", "script" };

class HTMLElement {
	public:
		Attributes attributes;
		vector<HTMLElement*> children;
		HTMLElement* parent = NULL;
		string type, text;

		HTMLElementType elementType;

		HTMLElement(const string& type, HTMLElementType elementType = NORMAL) :
			type(type),
			elementType(elementType) { }
		HTMLElement(const string& type, const Attributes& attributes, HTMLElementType elementType = NORMAL) :
			type(type),
			attributes(attributes),
			elementType(elementType) { }

		/*
            * The Core Interface *
			Interface for adding and removing HTMLElement from the list of child elements.
		*/

		void addElement(HTMLElement& element) {
		    addElement(&element);
		}
		void addElement(HTMLElement* element) {
			element->setParent(this);
			children.push_back(element);
		}

		void removeElement(HTMLElement& element) {
			int count = 0;
			for(HTMLElement* child : children) {
				if(child == &element) {
					child->parent = NULL;
					children.erase(children.begin() + count);
				}
				++count;
			}
		}
		void removeElement(HTMLElement* element) {
			int count = 0;
			for(HTMLElement* child : children) {
				if(child == element) {
					child->parent = NULL;
					children.erase(children.begin() + count);
				}
				++count;
			}
		}
        void setAttribute(const string& name, const string& value) {
            attributes[name] = value;
        }
        void setAttribute(const Attribute& attribute) {
            attributes.insert(attribute);
        }
        string getAttributeValue(const string& name) {
            string value;
            if(attributeExists(name))
                return attributes[name];

            return value;
        }
        bool removeAttribute(const string& name) {
            if(attributeExists(name)) {
                attributes.erase(name);
                return true;
            } else {
                return false;
            }
        }
		/*
			Node Functionality for changing and removing branches.
			Branches can directly be removed and changed using the following functions.
			addNode function will change the branch of the input element and set that element as its child.
		*/
		void addNode(HTMLElement& element) {
			if(element.parent == NULL) {
				addElement(element);
			} else {
				element.parent->removeElement(element);
				addElement(element);
			}
		}
		void deleteNode(HTMLElement& element) {
			removeElement(element);
		}

		/*
			Plain Text functions.
		*/
		string& getText() {
			return text;
		}
		void setText(const string& text) {
			this->text = text;
		}

		/*
			Functions for accessing and manipulating the parent of an HTMLElement.
		*/
		HTMLElement* getParent() {
			return parent;
		}
		void setParent(HTMLElement* parent) {
			this->parent = parent;
		}

		/*
			Function to display an HTMLElement in stdout
		*/
		void displayElement(const int& depth = 0, ostream& out = cout) {
			displayRecursive(this, depth, out);
		}

		bool attributeExists(const string& name) {
			return attributes.count(name) > 0;
		}

		/* Get the list of class names */
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

		/* Some searching functionality */
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

        /* Utilities for HTML Elements */
		int getChildIndex(HTMLElement* element) {
			int count = 0;
			for(HTMLElement* child : children) {
				if(child == element) {
					return count;
				}
				++count;
			}
			return -1;
		}

		string calculatePosition() {
			vector<string> path;
			stringstream ss;

			calculatePositionRecursive(this, path);

			int size = path.size();
			for(int i = size - 1; i >= 0; --i) {
				ss << path[i];
				if(i != 0) {
					ss << " > ";
				}
			}
			return ss.str();
		}
		void calculatePositionRecursive(HTMLElement* e, vector<string>& path) {
			if(e != NULL) {
				if(e->attributeExists("id")) {
					path.push_back("#" + e->attributes["id"]);
				} else if(e->attributeExists("class")) {
					vector<string> classes = e->getClasses();
					string allClasses;

					for(string& c : classes) {
						allClasses.append("." + c);
					}
					path.push_back(allClasses);
				} else {
					if(e->parent) {
						int location = e->parent->getChildIndex(e);
						if(location == -1) {
							path.push_back(e->type);
						} else {
							path.push_back(e->type + ":nth-child(" + to_string(location + 1) + ")");
						}
					} else {
						path.push_back(e->type);
					}
				}
				if(e->parent) {
					calculatePositionRecursive(e->parent, path);
				}
			}
		}

        int getDepth() {
            int depth = getDepthRecursive(this);
            return depth;
        }
        int getDepthRecursive(HTMLElement* element, int depth = 0) {
            if(element->parent == NULL) {
                return depth;
            }
            return getDepthRecursive(element->parent, depth + 1);
        }
	protected:
		HTMLElement* searchIdRecursive(HTMLElement* e, const string& id) {
			if(e->attributeExists("id")) {
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
		bool validateClasses(const vector<string>& parsedClassNames, const vector<string>& classes) {
			bool valid = true;
			bool found = false;
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
			return valid;
		}
		void searchClassRecursive(vector<HTMLElement*>& results, HTMLElement* e, const string& className, const vector<string>& parsedClassNames) {
			if(e != NULL) {
				if(className.empty() || parsedClassNames.empty())
					return;

				bool valid = true;
				if(e->attributes.count("class") > 0) {
					if(e->attributes["class"] == className) {
						results.push_back(e);
					} else {
						vector<string>&& classes = e->getClasses();
						valid = validateClasses(parsedClassNames, classes);
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
		void printSpaces(int depth, ostream& out) {
			for(int i = 0; i < depth; ++i) {
				out << INDENT;
			}
		}
		void printTagStart(HTMLElement* e, int depth, ostream& out) {
			printSpaces(depth, out);
			out << "<" << e->type;
			for(const Attribute& attribute : e->attributes) {
				out << ' ' << attribute.first << "=\"" << attribute.second << "\"";
			}
			if(e->elementType == NORMAL)
				out << ">";
		}
		void printChildren(HTMLElement* e, int depth, ostream& out) {
			for(HTMLElement* child : e->children) {
				out << endl;
				displayRecursive(child, depth + 1, out);
			}
		}
		void printText(HTMLElement* e, int depth, ostream& out) {
			if(!e->text.empty()) {
				out << endl;
                if(!isExcludedType(e))
                    printSpaces(depth + 1, out);
				out << e->text;
			}
		}
		void printTagEnd(HTMLElement* e, int depth, ostream& out) {
			if(e->elementType == NORMAL) {
				out << endl;
				printSpaces(depth, out);
				out << "</" << e->type << ">";
			} else {
				out << " />";
			}
		}
		void displayRecursive(HTMLElement* e, int depth, ostream& out) {
			if(e != NULL) {
				printTagStart(e, depth, out);
				if(e->elementType == NORMAL) {
					printChildren(e, depth, out);
					printText(e, depth, out);
				}
				printTagEnd(e, depth, out);
			}
		}
		void validate() {
			for(HTMLElement* child : children) {
				if(child != NULL) {
					child->parent = this;
				}
			}
		}
        bool isExcludedType(HTMLElement* element) {
            string& elementType = element->type;
            for(string& type : excludedTypes) {
                if(elementType == type) {
                    return true;
                }
            }
            return false;
        }
};
