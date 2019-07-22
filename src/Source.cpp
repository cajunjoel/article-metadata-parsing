#include"tinyxml2.h"
#include<string>
#include<iostream>
#include<algorithm>
#include<regex>

using namespace tinyxml2;
using namespace std;


void doWork();
void loadFile(int argc, char* argv[]);
string regEx(const string &hold);
void print();
void trim(string &str);
string capitalize(string &str);

int i = 0;

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

int main(int argc, char* argv[]) {

	loadFile(argc, argv);
	print();

	delete[] book;
	// system("pause");
	return 0;

}

void loadFile(int argc, char* argv[]) {

	XMLDocument doc;
	if(argc>0){

	XMLError loadOK = doc.LoadFile(argv[1]);

	cout << "FILE NAME :" << argv[1] << endl;

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
}


void doWork() {

	//iterates through rest of child nodes

	for (XMLNode* child = root->FirstChild(); child != NULL; child = child->NextSibling())
	{
		bool isArticle = false;

		if(strcmp(child->Value(), "author") == 0)
		{
			book[i].author == child->ToElement()->GetText();
		}

		if (strcmp(child->Value(), "figure") == 0 || strcmp(child->Value(), "construct") == 0 || strcmp(child->Value(), "table") == 0 || strcmp(child->Value(), "equation") == 0 || strcmp(child->Value(), "sectionHeader") == 0 || strcmp(child->Value(), "listItem") == 0 )   //sections where headers are located
		{
			string  tempstr = child->ToElement()->GetText();   //store all of text then narrow it down to 200 characters
			string str = tempstr.substr(0, 200);
			int authStart;

			for (authStart = 0; authStart < str.length(); authStart++)
			{
				if (str[authStart] == 'B' && (str[authStart + 1] == 'y' || str[authStart + 1] == 'v') && str[authStart + 2] == ' ' && child->NextSiblingElement("bodyText")) //if an author is in the title, author if next element is a body
				{
					tempstr = str.substr(0, authStart);            //store string until 'B' and 'y'
					string auth = str.substr(authStart - 1, str.size() - authStart);    //store string after'B' 'y'
				
					book[i].author = regEx(auth);
					
					book[i].startPageID = child->ToElement()->Attribute("page_id");
					book[i].startPage = child->ToElement()->Attribute("page_num");

					isArticle = true;

					break;

				}

			}// this works

			for (int headstart = 0; headstart < tempstr.length(); headstart++)
			{
				if (isupper(tempstr[headstart])&& (isupper(tempstr[headstart+1]) || tempstr[headstart+1] == ' ' ) && isArticle==true)   //double checking to make sure only uppercase headers that are an article are stored
				{
					tempstr = tempstr.substr(headstart, authStart);
					trim(tempstr);
					capitalize(tempstr);
					book[i].header = tempstr;
					break;
				}

			}
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

					book[i].author = regEx(hold);
					trim(str);
					capitalize(str);

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


		if (strcmp(child->Value(), "bodyText") == 0 || strcmp(child->Value(), "keyword") == 0 || strcmp(child->Value(), "construct") == 0)   //sections where date, issue and vol are
		{

			string biginfo = child->ToElement()->GetText();
			string info = biginfo.substr(0, 60);
			string anothertemp;
			
			for (int authChar = 0; authChar < info.length(); authChar++)
			{
				if ((info[authChar] == 'N' && info[authChar + 1] == 'o' && (info[authChar + 2] == ' ' || info[authChar + 2] == '.'))    //issues are marked by "NO., No. or No "
					|| (info[authChar] == 'N' && info[authChar + 1] == 'O' && info[authChar + 2] == '.'))
				{
					
					anothertemp= info.substr(authChar + 6, 18);
					trim(anothertemp);

					if (anothertemp[0] == '.' || anothertemp[0] == ' ')    //gets rid of period or space stored before some dates
						anothertemp = anothertemp.substr(1, anothertemp.length() - 1);

					book[i].date = capitalize(anothertemp);

					anothertemp = info.substr(authChar, 6);
					trim(anothertemp);
					book[i].issue = capitalize(anothertemp);

				}
			}

			for (int authChar = 0; authChar < biginfo.length(); authChar++)
			{
				if ((biginfo[authChar] == 'V' || biginfo[authChar] == 'Y'  )&& (biginfo[authChar + 1] == 'O') && (biginfo[authChar + 2] == 'L' || biginfo[authChar + 2] == 'I'))   //volumes are marked by "VOL" or "VOI"
				{
					anothertemp=biginfo.substr(authChar, 8);
					trim(anothertemp);
					book[i].volume = anothertemp;
				}
			}
		}

		if (isArticle == true)  //only increase i if articles are found
			i++;
	}
}

 
string regEx(const string &hold) {    //to find authors, use reg expression

	// cout << "Looking at: ---" << hold << "---" << endl;
	try {
		regex r("B[yv] ([a-z.,-^ ]*?) ?(,|\\.$).*?$");
		regex r2("B[yv] ([a-z.,-^ ]*?)[ .,]?[\n\r]+");
		regex r3("B[yv] ([a-z.,-^ ]*?)[\n\r]+");
		smatch match;

		if (regex_search(hold, match, r)) {
			return match[1];
			// cout << "Working: " << hold << " --> " << match[1] << endl;
		} else if (regex_search(hold, match, r2)) {
			return match[1];
		} else {
			return string("");
			// cout << " not Working: " << hold << endl;
		}

	} catch (regex_error& e) {
		return string("");
		// cout << " error!" << string("") << endl;
	}

	return string("");
}

	
void print() {

	for (int c = 0; c < i; c++)
	{
		if (book[c].date == "")
		{
			book[c].date = capitalize(book[c - 1].date);
			book[c].issue = capitalize(book[c - 1].issue);
		}

		if(book[c].volume == "")
		book[c].volume = book[c - 1].volume;

		cout << "\"" << c << "\"\t\"" << book[c].header << "\"\t\"" << book[c].author << "\"\t\"" << book[c].date << "\"\t\"" << book[c].volume << "\"\t\"" << book[c].issue << "\"\t\"" << book[c].startPageID << "\"\t\"" << book[c].endPageID << "\"\t\"" << book[c].startPage << "\"\t\"" << book[c].endPage << "\"" <<  endl;
		// cout << book[i].header << endl;
	}

}
 
void trim(string& str)     //trims away new line characters in xml
{
	str.erase(remove(str.begin(), str.end(), '\n'), str.end());
}

string capitalize(string &str)
{

	if (!str.empty())
	{
		str[0] = toupper(str[0]);

		for (size_t i = 1; i < str.length(); ++i)
		{
			str[i] = tolower(str[i]);
			if (str[i - 1] == ' ' || str[i-1] == '-' || str[i-1] =='.')
				str[i] = toupper(str[i]);
		}
	}

	
	return str;
}