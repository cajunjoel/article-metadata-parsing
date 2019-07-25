#include"tinyxml2.h"
#include<string>
#include<iostream>
#include<algorithm>
#include<cstddef>
#include<regex>

using namespace tinyxml2;
using namespace std;


void doWork();
void loadFile(int argc, char* argv[]);
string findAuthor(const string &hold);
void print();
string trim(string str);
string capitalize(string str);
void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr);
string fixRomanNumerals(std::string str);

int i = 0;

XMLElement* root;

struct magazine {

	string author;
	string header;
	string date;
	string volume;
	string issue;
	string number;
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
	return 0;

}

void loadFile(int argc, char* argv[]) {

	XMLDocument doc;
	if(argc>0){
	
		XMLError loadOK = doc.LoadFile(argv[1]);

		if (loadOK == XML_SUCCESS)
		{
			root = doc.FirstChildElement("algorithms");  //for finding root element

			if (root != NULL)
			{
				doWork();
			}

			else
				cerr << "root is null " << endl;
			
		}

	else
		cerr << "Error loading file " << endl;
	}
}

/* 
Function: doWork()

Cycles through XML file to extract the article information

*/
void doWork() {

	//iterates through all of child nodes
	for (XMLNode* child = root->FirstChild(); child != NULL; child = child->NextSibling())
	{
		bool isArticle = false;
		smatch match;

		// This code may not be used 
		if(strcmp(child->Value(), "author") == 0)
		{
			book[i].author = child->ToElement()->GetText();
		}
		// Decide if this is a section header and grab info 
		if (strcmp(child->Value(), "figure") == 0 || strcmp(child->Value(), "construct") == 0 || strcmp(child->Value(), "table") == 0 
		    || strcmp(child->Value(), "equation") == 0 || strcmp(child->Value(), "sectionHeader") == 0 
			|| strcmp(child->Value(), "listItem") == 0 )   //sections where headers are located
		{
			string tempstr = child->ToElement()->GetText();   //store all of text then narrow it down to 200 characters
			string str = tempstr.substr(0, 200);
			// cerr << "str: " << str << endl; 
			int authStart;

			XMLElement* sib = child->NextSiblingElement();
			// cerr << "str.length(): " << str.length() << endl;

			// Extract the author
			if (sib) {
				if (str.length() > 3) {
					for (authStart = 0; authStart < (str.length() - 3) ; authStart++)
					{
						// cerr << "here0" << endl;
						if (str[authStart] == 'B' 
							&& (str[authStart + 1] == 'y' || str[authStart + 1] == 'v') 
							&& str[authStart + 2] == ' ' 
							&& strcmp(sib->Value(), "bodyText") == 0) 
							// if an author is in the title, it's the author of the next bodytext element
						{
							tempstr = str.substr(0, authStart);            //store string until 'B' and 'y'
							string auth = str.substr(authStart - 1, str.length() - authStart);    //store string after'B' 'y'
						
							book[i].author = capitalize(trim(findAuthor(auth)));
							
							book[i].startPageID = child->ToElement()->Attribute("page_id");
							book[i].startPage = child->ToElement()->Attribute("page_num");

							isArticle = true;

							break;

						}

					}// this works

				}

				for (int headstart = 0; headstart < tempstr.length(); headstart++)
				{
					if (isupper(tempstr[headstart])&& (isupper(tempstr[headstart+1]) || tempstr[headstart+1] == ' ' ) && isArticle==true)   //double checking to make sure only uppercase headers that are an article are stored
					{
						tempstr = tempstr.substr(headstart, authStart);
						tempstr = trim(tempstr);
						tempstr = capitalize(tempstr);
						book[i].header = tempstr;
						break;
					}

				}
				// if author is in body
				string temps;
				string s;
			
				if (strcmp(sib->Value(), "bodyText") == 0)
				{
					temps = child->NextSiblingElement()->GetText();
					s = temps.substr(0, 75);

					for (int authChar = 0; authChar < s.length(); authChar++)  //if author is in the body
					{
						if (s[authChar] == 'B' && (s[authChar + 1] == 'y'))
						{
							string hold = s.substr(authChar, 25);

							book[i].author = capitalize(trim(findAuthor(hold)));
							book[i].header = capitalize(trim(str));

							book[i].startPageID = child->ToElement()->Attribute("page_id");
							book[i].startPage = child->ToElement()->Attribute("page_num");

							isArticle = true;

							break;
						}
					}

				}
				
			}

			regex titleRegex(".*?All Rights Reserved. [A-Za-z]+, [0-9]{4}[.,]?(.+)");
			// regex titleRegex("(All Rights Reserved. [A-Za-z]+, [0-9]{4})");
			cerr << "TITLE BEFORE: " << book[i].header << endl ;
			if (regex_search(book[i].header, match, titleRegex)) {
				cerr << "TITLE Regex Matched: " << match[1].str() << endl ;
				book[i].header = capitalize(trim(match[1].str()));
			}
			// book[i].header = fixRomanNumerals(book[i].header);

			//when the next section header, the article ends // so end page # and page id
			if (i >= 1 && isArticle)
			{
				book[i-1].endPage = child->ToElement()->Attribute("page_num");
				book[i-1].endPageID = child->ToElement()->Attribute("page_id");
			}
		}

		// Decide if this is a body of text and grab author, date, issue, number.
		if (strcmp(child->Value(), "bodyText") == 0 || strcmp(child->Value(), "keyword") == 0 || strcmp(child->Value(), "construct") == 0)   //sections where date, issue and vol are
		{
			string biginfo = child->ToElement()->GetText();
			cerr << "Biginfo ---- " << endl << biginfo << endl << "----------- "  << endl << endl;

			regex dateRegex("([A-Z]+),( [0-9]{4})");
			if (regex_search(biginfo, match, dateRegex)) {
				cerr << "DATE Regex Matched: " << match[1].str() << match[2].str() << endl ;
				book[i].date = capitalize(trim(match[1].str())) + match[2].str();
			}

			regex volRegex("([VY]O[LI]v?[,.] [IVXLCivxlc0-9]+)");
			if (regex_search(biginfo, match, volRegex)) {
				cerr << "VOL Regex Matched: " << match[1].str() << endl ;
				book[i].volume = trim(match[1].str());
				findAndReplaceAll(book[i].volume, "VOIv", "VOL");
				book[i].volume = capitalize(book[i].volume);
				book[i].volume = fixRomanNumerals(book[i].volume);
			}

			regex numberRegex("(N[oO][.,]? [IVXLCivxlc0-9]+)");
			if (regex_search(biginfo, match, numberRegex)) {
				cerr << "NUM Regex Matched: " << match[1].str() << endl ;
				book[i].number = capitalize(trim(match[1].str()));
				book[i].number = fixRomanNumerals(book[i].number);
			}

			cerr << "BOOK INFO : " << book[i].number << " | " << book[i].date << " | " << book[i].volume << endl;
		}

		if (isArticle == true)  //only increase i if articles are found
			i++;
	}
}

 
string findAuthor(const string &hold) {    //to find authors, use reg expression

	cerr << "Looking at: ---" << hold << "---" << endl;
	try {
		regex r("B[yv] ([a-z.,-^ ]*?) ?(,|\\.$).*?$");
		regex r2("B[yv] ([a-z.,-^ ]*?)[ .,]?[\n\r]+");
		regex r3("B[yv] ([a-z.,-^ ]*?)[\n\r]+");
		
		smatch match;

		if (regex_search(hold, match, r)) {
			cerr << "Found R: " << match[1] << endl;
			return match[1];
		} 
		else if (regex_search(hold, match, r2)) {
			cerr << "Found R2: " << match[1] << endl;
			return match[1];
		} 
		else {
			cerr << "Found Nothign " << endl;
			return string("");
			// cerr << " not Working: " << hold << endl;
		}

	} catch (regex_error& e) {
		return string("");
		// cerr << " error!" << string("") << endl;
	}

	return string("");
}

	
void print() {
	// cout << "Title" << "\t" 
    // 	<< "Author" << "\t" << "Date" << "\t" 
	//     << "Volume" << "\t" << "Number" << "\t" << "Issue" << "\t" 
	// 	<< "StartPageID" << "\t" << "EndPageID" << "\t" 
	// 	<< "StartPage" << "\t" << "EndPage" <<  endl;

	for (int c = 0; c < i; c++)
	{
		if (book[c].date == "")
		{
			book[c].date = book[c - 1].date;
			book[c].issue = book[c - 1].issue;
			book[c].number = book[c - 1].number;
		}

		if(book[c].volume == "")
		book[c].volume = book[c - 1].volume;

		cout << book[c].header << "\t" 
		     << book[c].author << "\t" << book[c].date << "\t" 
			 << book[c].volume << "\t" << book[c].number << "\t" << book[c].issue << "\t" 
			 << book[c].startPageID << "\t" << book[c].endPageID << "\t" 
			 << book[c].startPage << "\t" << book[c].endPage <<  endl;

	}

}
 
//trims away new line characters in xml
string trim(string str)     
{
	try {
		regex rLeft("^[\r\n .]+");
		str = regex_replace(str, rLeft, "");

		regex rRight("[\r\n .]+$");
		str = regex_replace(str, rRight, "");

		regex rNewline("[\r\n]+");
		str = regex_replace(str, rNewline, " ");

		return str;
	} catch (exception& e) {
		cerr << "exception caught: " << e.what() << " trying to parse " << str << "\n"; 
		return "";
	}
}

string capitalize(string str)
{
	try {
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
	} catch (exception& e) {
		cerr << "exception caught: " << e.what() << "\n"; 
		return "";
	}

	return str;
}


void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
 
	// Repeat till end is reached
	while( pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

string fixRomanNumerals(std::string str) {
	smatch match;

	regex r("([IVXLCivxlc]{2,})");
	if (regex_search(str, match, r)) {
		string u = match[1].str();

		for (size_t i = 1; i < u.length(); ++i) {
			u[i] = toupper(u[i]);
		}
		str = regex_replace(str, r, u);
	}

	return str;
}