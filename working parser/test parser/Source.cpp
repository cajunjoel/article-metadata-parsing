#include"tinyxml2.h"
#include<string>
#include<iostream>
#include<algorithm>
#include<regex>


using namespace tinyxml2;
using namespace std;


void doWork();
void loadFile();
string regEx(string hold);
void print();
void trim(string &str);

int i = 0;
int r = 0;

XMLElement* root;

struct magazine {

	string author;
	string header;
	string date;
	string volume;
	string issue;
	string startPage;
	string endPage;
	string startPageID;
	string endPageID;

};

magazine* book = new magazine[500];

int main() {

	loadFile();
	print();

	delete[] book;

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
		bool isArticle = false;

		if (strcmp(child->Value(), "figure") == 0 || strcmp(child->Value(), "construct") == 0 || strcmp(child->Value(), "table") == 0 || strcmp(child->Value(), "equation") == 0 || strcmp(child->Value(), "sectionHeader") == 0)
		{
			string  tempstr = child->ToElement()->GetText();
			string str = tempstr.substr(0, 200);

			for (int authStart = 0; authStart < str.length(); authStart++)
			{
				if (str[authStart] == 'B' && (str[authStart + 1] == 'y' || str[authStart + 1] == 'v') && str[authStart + 2] == ' ' && child->NextSiblingElement("bodyText")) //if an author is in the title, author if next element is a body
				{

					tempstr=str.substr(0, authStart);   //tempstr reassigned
					string hold = str.substr(authStart - 1, str.size() - authStart);
					//tempstr.erase(remove(tempstr.begin(), tempstr.end(), '\n'), tempstr.end());
					trim(tempstr);
					book[i].header = tempstr;

					regEx(hold);

					book[i].startPageID = child->ToElement()->Attribute("page_id");
					book[i].startPage = child->ToElement()->Attribute("page_num");

					isArticle = true;

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

			for (int authChar = 0; authChar < s.length(); authChar++)  //if author is in the body
			{
				if (s[authChar] == 'B' && (s[authChar + 1] == 'y'))
				{

					string hold = s.substr(authChar, 25);

					regEx(hold);

					trim(str);

					book[i].header = str;


					book[i].startPageID = child->ToElement()->Attribute("page_id");
					
					book[i].startPage = child->ToElement()->Attribute("page_num");

					isArticle = true;

					break;
				}
			}

			//when the next section header, the article ends // so end page # and page id
			if (i >= 1 && isArticle)
			{
				book[i-1].endPage = child->ToElement()->Attribute("page_num");
				book[i-1].endPageID = child->ToElement()->Attribute("page_id");
			}
		}


		if (strcmp(child->Value(), "bodyText") == 0 || strcmp(child->Value(), "keyword") == 0 || strcmp(child->Value(), "construct") == 0)
		{

			string biginfo = child->ToElement()->GetText();
			string info = biginfo.substr(0, 60);
			string anothertemp;
			
			for (int authChar = 0; authChar < info.length(); authChar++)
			{
				if ((info[authChar] == 'N' && info[authChar + 1] == 'o' && (info[authChar + 2] == ' ' || info[authChar + 2] == '.')) 
					|| (info[authChar] == 'N' && info[authChar + 1] == 'O' && info[authChar + 2] == '.'))
				{
					anothertemp= info.substr(authChar + 6, 18);
					trim(anothertemp);
					book[i].date = anothertemp;

					anothertemp = info.substr(authChar, 6);
					trim(anothertemp);
					book[i].issue = anothertemp;

				}
			}

			for (int authChar = 0; authChar < biginfo.length(); authChar++)
			{
				if (biginfo[authChar] == 'V' && (biginfo[authChar + 1] == 'O') && (biginfo[authChar + 2] == 'L' || biginfo[authChar + 2] == 'I'))
				{
					anothertemp=biginfo.substr(authChar, 8);
					trim(anothertemp);
					book[i].volume = anothertemp;
				}
			}
		}

		if (isArticle == true)
			i++;
	}
}


string regEx(string hold) {

	try {
		regex r("By ([a-z.,-^ ]*?) ?(,|\\.$).*?$");
		smatch match;

		if (regex_search(hold, match, r) && match.size() > 1)
			book[i].author = match.str(1);
		else
			book[i].author = string("");

	}
	catch (regex_error& e)
	{
		cout << string("") << endl;
	}

	return book[i].author;
}

	
void print() {

	for (int c = 0; c < i; c++)
	{
		if (book[c].date == "")
		{
			book[c].date = book[c - 1].date;
			book[c].issue = book[c - 1].issue;
		}

		if(book[c].volume == "")
		book[c].volume = book[c - 1].volume;

		cout << "\"" << book[c].header << "\", \"" << book[c].author << "\", \"" << book[c].date << "\", \"" << book[c].volume << "\", \"" << book[c].issue << "\", \"" << book[c].startPageID << "\", \"" << book[c].endPageID << "\", \"" << book[c].startPage << "\", \"" << book[c].endPage << "\"" <<  endl;
		
	}

}

void trim(string& str)
{
	str.erase(remove(str.begin(), str.end(), '\n'), str.end());
}