#include"tinyxml2.h"
#include<string>
#include<iostream>
#include<algorithm>


using namespace tinyxml2;
using namespace std;


void doWork();
void loadFile();


int i = 0;

XMLElement* root;

struct {

	string author[200];
	string header[200];
	string date[50];
	string volume;
	string issue[50];

	string pageID[500];
	

} book;


int main() {

	loadFile();

	system("pause");
	return 0;

}

void loadFile() {

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
			string  tempstr = child->ToElement()->GetText();
			string str = tempstr.substr(0, 75);
			//cout << str << endl;

			for (int authStart = 0; authStart < str.length(); authStart++)
			{

				if (str[authStart] == 'B' && (str[authStart + 1] == 'y' || str[authStart + 1] == 'v') && str[authStart + 2] == ' ' && child->NextSiblingElement("bodyText")) //if an author is in the title, author if next element is a body
				{

					book.header[i] = str.substr(0, authStart);
					book.author[i] = str.substr(authStart - 1, str.size() - authStart);
					book.pageID[i] = child->ToElement()->Attribute("page_id");    

					cout << "1 ---------------------------------------------- " << endl;
					cout << "title: " << book.header[i] << endl;
					cout << "author: " << book.author[i] << endl;
					cout << "page ID: " << book.pageID[i] << endl;
					cout << endl;

					break;

				}

			}    // this works

			// if author is in body
			string temps;
			string s;

			if (child->NextSiblingElement("bodyText") != NULL)
			{
				temps = child->NextSiblingElement("bodyText")->GetText();
				s = temps.substr(0, 75);

			}
			//cout << s << endl;

			for (int authChar = 0; authChar < s.length(); authChar++)  //if author is in the body
			{

				if (s[authChar] == 'B' && (s[authChar + 1] == 'y'))
				{
					book.author[i] = s.substr(authChar, 25);  //allows for 25 characters. how to determine end?
					book.header[i] = str;
					book.pageID[i] = child->ToElement()->Attribute("page_id");    

					cout << "2 ---------------------------------------------- " << endl;
					cout << "title: " << book.header[i] << endl;
					cout << "author: " << book.author[i] << endl;
					cout << "page ID: " << book.pageID[i] << endl;
					cout << endl;

					
					break;
				}

			}


		}



		 if (strcmp(child->Value(), "bodyText") == 0 || strcmp(child->Value(), "keyword") ==0 || strcmp(child->Value(), "construct") == 0)
		{
			string biginfo = child->ToElement()->GetText();
			string info = biginfo.substr(0, 60);

			for (int authChar = 0; authChar < info.length(); authChar++)
			{
				if ((info[authChar] == 'N' && info[authChar + 1] == 'o' && (info[authChar + 2] == ' ' || info[authChar + 2] == '.')) || (info[authChar] == 'N') && (info[authChar + 1] == 'O' && info[authChar + 2] == '.'))
				{


					book.date[i] = info.substr(authChar + 6, 18);

					book.issue[i] = info.substr(authChar, 6);

					cout << "------------------------------------------------ " << endl << endl;
					cout << "Date: " << book.date[i] << endl;
					cout << "Issue: " << book.issue[i] << endl;


				}

			}

			for(int authChar=0; authChar <biginfo.length(); authChar++)
			{
				if (biginfo[authChar] == 'V' && (biginfo[authChar + 1] == 'O') && (biginfo[authChar +2] == 'L' || biginfo[authChar+2] =='I'))
				{

					book.volume = biginfo.substr(authChar, 8);
					cout << "Volume: " << book.volume << endl;
				}

			}

		}

		i++;

	}
}




	
