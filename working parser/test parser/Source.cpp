#include"tinyxml2.h"
#include<string>
#include<iostream>
#include<algorithm>


using namespace tinyxml2;
using namespace std;


void doWork();
void testFile();


int i = 0;

XMLElement* root;

struct {

	string author[200];
	string header[200];
	string date[50];
	string volume[20];
	string issue[50];

	char *pageid;
	
} book;


int main() {

	XMLElement* root = 0;

	testFile();

	/*for (int u = 0; u < i; u++)
	{

		cout << "title: " << book.header[u] << endl;
		cout << "author: " << book.author[u] << endl;
		cout << "page ID: " << book.pageID[u] << endl;
		cout << "Volume: " << book.volume[u] << endl;
		cout << "Date: " << book.date[u] << endl;
		cout << "Issue: " << book.issue[u] << endl;
	}
	*/

	system("pause");
	return 0;

}

void testFile() {

	XMLDocument doc;
	XMLError loadOK = doc.LoadFile("aviculturalm118941895avic.xml");
	
	

	if (loadOK == XML_SUCCESS)
	{
		 root = doc.FirstChildElement("algorithms");  //for finding root element

		if (root != NULL)
		{

			doWork();
	
		}

		else
			cout << "root is null " << endl;
		
	}

	else
		cout << "Error loading file " << endl;

}


void doWork() {

	//iterates through rest of child nodes



	for (XMLNode* child = root->FirstChild(); child != NULL; child = child->NextSibling())
	{

		if (strcmp(child->Value(), "figure") == 0 || strcmp(child->Value(), "construct") == 0 || strcmp(child->Value(), "table") == 0 || strcmp(child->Value(), "equation") == 0 || strcmp(child->Value(), "sectionHeader") == 0)
		{
			string tempstr = child->ToElement()->GetText();
			// string foo = child->ToElement()->Attribute("page_id");

			//cout << "[= CHILD " << child->Value() << "=]" << endl;

			string str = tempstr.substr(0, 75);
			//cout << "[=" << str << "=]" << endl;

			for (int authStart = 0; authStart < str.length(); authStart++)
			{

				if (str[authStart] == 'B' && str[authStart + 1] == 'y')  //if an author is in the title
				{

					cout << "1 ---------------------------------------------- " << endl;
					book.header[i] = str.substr(0, authStart);
					cout << "header: " << book.header[i] << endl;
					
					book.author[i] = str.substr(authStart, str.size() - authStart);
					cout << "author: " << book.author[i] << endl;

					const char* pageid = child->ToElement()->Attribute("page_id");
					book.pageid[i] = pageid;
					cout << "page ID: (" << pageid << ") Length: " << endl;

					break;
				}

			}

			// if author is in body


			/*
			string temps = child->NextSiblingElement("bodyText")->GetText();

			string s = temps.substr(0, 75);


				for (int authChar = 0; authChar < s.length(); authChar++)  //if author is in the body
				{

					if (s[authChar] == 'B' && (s[authChar + 1] == 'y'))
					{
						book.author[i] = s.substr(authChar, 25);  //allows for 25 characters. how to determine end?
						book.header[i] = str;
						book.pageID[i] = child->NextSiblingElement("sectionHeader")->Attribute("page_id");

						cout << "2 ---------------------------------------------- " << endl;
						cout << "title: " << book.header[i] << endl;
						cout << "author: " << book.author[i] << endl;
						cout << "page ID: " << book.pageID[i] << endl;

						break;
					}
				}

		}*/
		/*

		else if (strcmp(child->Value(), "bodyText") == 0 || strcmp(child->Value(), "keyword") == 0)
		{
			string info;
			info = child->ToElement()->GetText();


			for (int authChar = 0; authChar < info.length(); authChar++)
			{
				if (info[authChar] == 'N' && (info[authChar + 1] == 'O'))
				{

					book.date[i] = info.substr(8, 25);

					book.issue[i] = info.substr(authChar, 5);

					cout << endl;
					cout << "Date: " << book.date[i] << endl;
					cout << "Issue: " << book.issue[i] << endl;

				}

				else if (info[authChar] == 'V' && (info[authChar + 1] == 'O'))
				{

					book.volume[i] = info.substr(authChar, 5);
					cout << "Volume: " << book.volume << endl;
				}

			}
		}
				*/
		}
		i++;
	}
}




	
