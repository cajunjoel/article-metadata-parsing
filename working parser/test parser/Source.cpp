#include"tinyxml2.h"
#include"dirent.h"
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

	string pageID[200];


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


void doWork(XMLNode* parent) {

	//iterates through rest of parent nodes

	for (parent = root->FirstChild(); parent != NULL; parent = parent->NextSibling())
	{


		if (!(parent->NoChildren())) //if have children
		{

			bool hasAuthor = false;
			bool titleAuthor = false;

			string st;
			
			string temp;

			//storing strings for checking
			string str;

			if (parent->NextSiblingElement("figure"))
				str = parent->NextSiblingElement("figure")->GetText();

			 if (parent->NextSiblingElement("construct"))
				str = parent->NextSiblingElement("construct")->GetText();

			 if (parent->NextSiblingElement("table"))
				str = parent->NextSiblingElement("table")->GetText();

			 if (parent->NextSiblingElement("equation"))
				 str = parent->NextSiblingElement("equation")->GetText();


			if (parent->NextSiblingElement("sectionHeader"))
				str = parent->NextSiblingElement("sectionHeader")->GetText();

		


			for (int authStart = 0; authStart < str.length(); authStart++)
			{
				if (str[authStart] == 'B' && str[authStart + 1] == 'y')  //if an author is in the title
				{
					st = str.substr(0, authStart);
					titleAuthor = true;
					hasAuthor = true;
					book.author[i] = str.substr(authStart, str.size() - authStart);
					book.header[i] = st;
					book.pageID[i] = parent->NextSiblingElement("sectionHeader")->Attribute("page_id");

					cout << "1 ---------------------------------------------- " << endl;
					cout << "title: " << book.header[i] << endl;
					cout << "author: " << book.author[i] << endl;
				//	cout << "page ID: " << book.pageID[i] << endl;
				}

			}
		
						
			if (titleAuthor == false) {   //if author is in the body

				string s;

				s = parent->NextSiblingElement("bodyText")->GetText();


				for (int authChar = 0; authChar < s.length(); authChar++)
				{
					if (s[authChar] == 'B' && (s[authChar + 1] == 'y'))
					{
						hasAuthor = true;
						book.author[i] = s.substr(authChar, 25);  //allows for 25 characters. how to determine end?
						book.header[i] = str;


						cout << "2 ---------------------------------------------- " << endl;
						cout << "title: " << book.header[i] << endl;
						cout << "author: " << book.author[i] << endl;
						book.pageID[i] = parent->NextSiblingElement("sectionHeader")->Attribute("page_id");

						cout << "page ID: " << book.pageID[i] << endl;


						break;
					}

				}
			}
				string info;

				info = parent->NextSiblingElement("bodyText")->GetText();


				for (int authChar = 0; authChar < info.length(); authChar++)
				if (info[authChar] == 'N' && (info[authChar + 1] == 'O'))
				{

					book.date[i] = info.substr(8, 25);

					book.issue[i] = info.substr(authChar, 5);

					cout << endl;
				//	cout << "Date: " << book.date[i] << endl;
					//cout << "Issue: " << book.issue[i] << endl;

				}

				else if (info[authChar] == 'V' && (info[authChar + 1] == 'O'))
				{

					book.volume[i] = info.substr(authChar, 5);
					//cout << "Volume: " << book.volume << endl;
				}

				

				i++;
			
		}
	}

	cout << " All of the metadata " << endl;
}
