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
void findAndReplaceAll(string & data, string toSearch, string replaceStr);
string fixRomanNumerals(string str);

int counter = 0;    //starts counter for number of articles

XMLElement* root;  //first element of the xml file
/*

struct to introduce the elements of the object

 */
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
magazine* article = new magazine[500];   // create an article

/*
main() loads file, prints results, and deletes memory

 */
int main(int argc, char* argv[]) {

	loadFile(argc, argv);
	print();                      //printing the results

	delete[] article;             //freeing the allocated memory
	return 0;

}

/*

loadFile(int argc, char* argv[]) checks the arguments passed by the user, and if valid, then runs through the doWork() function

 */
void loadFile(int argc, char* argv[]) {

	XMLDocument doc;
	if(argc>0){                  							//if the user enters at least one argument
	
		XMLError loadOK = doc.LoadFile(argv[1]);            //load the file that the user requested

		if (loadOK == XML_SUCCESS)          				 //if load is successful, then find the root element
		{
			root = doc.FirstChildElement("algorithms");  	//for finding root element

			if (root != NULL)
			{
				doWork();                  					//if there is a root and it has children, loop through the children in doWork() function
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

Cycles through XML file to extract the article information (children and siblings)

*/
void doWork() {
 //cout << "HERE0" << endl;
	//iterates through all of child nodes
	for (XMLNode* child = root->FirstChild(); child != NULL ; child = child->NextSibling())            //while child is not null, loop through all the children nodes starting at the first one
	{

		// cout << "HERE1" << endl;
		//if(child->NextSibling() != NULL)
		//{cout << "looking at next sibling" << child->NextSibling() << endl;

//		cout << "what is next sibling " << child->NextSibling()->ToElement()->GetText() << endl;
//		}

		bool isArticle = false;          							//default false until conditions are met
		smatch match;

		// This code may not be used 
		if(strcmp(child->Value(), "author") == 0)                    //if the child tag is <author>,  store it
		{
		 article[counter].author = child->ToElement()->GetText();
		}

		// Decide if this is a section header and grab info 
		if (strcmp(child->Value(), "figure") == 0 || strcmp(child->Value(), "construct") == 0 || strcmp(child->Value(), "table") == 0 
		    || strcmp(child->Value(), "equation") == 0 || strcmp(child->Value(), "sectionHeader") == 0 
			|| strcmp(child->Value(), "listItem") == 0 )   //sections where headers are located
		{
			string tempstr = child->ToElement()->GetText();   		//store all of text then narrow it down to 200 characters
			string str = tempstr.substr(0, 200);
			// cerr << "str: " << str << endl; 
			int authStart;


			XMLElement* sib = child->NextSiblingElement();         //sibling element introduced to loop through
			// cerr << "str.length(): " << str.length() << endl;
			 //err << "str: " << str << endl;

			// Extract the author
			if (sib!=NULL) {
				if (str.length() > 3) {     					//to keep loop going (so the length value is never negative)
					for (authStart = 0; authStart < (str.length()-3 ) ; authStart++)  
					{
						 //cerr << "here0" << endl;
						if (str[authStart] == 'B' 
							&& (str[authStart + 1] == 'y' || str[authStart + 1] == 'v') 
							&& str[authStart + 2] == ' ' 
							&& strcmp(sib->Value(), "bodyText") == 0) 
															// if an author is in the title, marked by "By", it's the author of the next bodytext element
						{
							tempstr = str.substr(0, authStart);            //store string until 'B' and 'y'
							string auth = str.substr(authStart - 1, str.length() - authStart);    //store string after'B' 'y'
						
						 article[counter].author = capitalize(trim(findAuthor(auth)));             //runs thru capitalize, trim, and findAuthor functions and stores            
							
						 article[counter].startPageID = child->ToElement()->Attribute("page_id");                  //looks at attributes to store page ids and numbers
						 article[counter].startPage = child->ToElement()->Attribute("page_num");

							isArticle = true;                 //it is an article

							break;

						}

					}// this works

				}//out << "HERE?" << endl;
				for (int headstart = 0; headstart < tempstr.length(); headstart++)
				{								
													//double checking to make sure only uppercase headers that are an article are stored
					if (isupper(tempstr[headstart])&& (isupper(tempstr[headstart+1]) || tempstr[headstart+1] == ' ' ) && isArticle==true)   //if at least first two characters are uppercase and it is an article, store
					{
						tempstr = tempstr.substr(headstart, authStart);         //from start of Cap letters to the start of the "By"
						tempstr = trim(tempstr);                                //runs thru trim function, then capitalize function, then store
						tempstr = capitalize(tempstr);
					 article[counter].header = tempstr;
						break;
					}

				}
				// if author is in body
				string temps;
				string s;
			
				if (strcmp(sib->Value(), "bodyText") == 0 ||strcmp(sib->Value(), "construct") == 0)
				{
					// cout << "HERE" << endl;
					temps = child->NextSiblingElement()->GetText();              //store bodytext, then only 75 characters of that into s
					s = temps.substr(0, 75);
	
					for (int authChar = 0; authChar < s.length(); authChar++)  //if author is in the body, it will be within the first 75 characters
					{
						if (s[authChar] == 'B' && (s[authChar + 1] == 'y'))
						{
							string hold = s.substr(authChar, 25);

						 article[counter].author = capitalize(trim(findAuthor(hold))); 
						   //runs thru capitalize, trim, findAuthor functions
						 article[counter].header = capitalize(trim(str));                //runs thru capitalize and trim funcitions

						 article[counter].startPageID = child->ToElement()->Attribute("page_id");      //same as above
						 article[counter].startPage = child->ToElement()->Attribute("page_num");

							isArticle = true;

							break;
						}
					}

				}
				
			}
 

			regex titleRegex(".*?All Rights Reserved. [A-Za-z]+, [0-9]{4}[.,]?(.+)");            //some titles store this information, so weeding it out via regEx
			// regex titleRegex("(All Rights Reserved. [A-Za-z]+, [0-9]{4})");
			//cerr << "TITLE BEFORE: " << article[counter].header << endl ;
			if (regex_search (article[counter].header, match, titleRegex)) {                    //if found, then remove irrelevant parts that are not title, and store
				//cerr << "TITLE Regex Matched: " << match[1].str() << endl ;
			 article[counter].header = capitalize(trim(match[1].str()));
			  //cerr << "here0" << endl;
			}
			// article[counter].header = fixRomanNumerals article[counter].header);
 
			//when the next section header begins, the last article ends // so end page # and page id of the previous article is the beginning of the next
			if (counter >= 1 && isArticle)
			{
			
			 article[counter-1].endPage = child->ToElement()->Attribute("page_num");
			 article[counter-1].endPageID = child->ToElement()->Attribute("page_id");
			}
		}

		// Decide if this is a body of text and grab author, date, issue, number.
		if (strcmp(child->Value(), "bodyText") == 0 || strcmp(child->Value(), "keyword") == 0 || strcmp(child->Value(), "construct") == 0)   //sections where date, issue and vol are
		{
			string biginfo = child->ToElement()->GetText();
			//cerr << "type: " << child->Value() << " Biginfo ---- " << endl << biginfo << endl << "----------- "  << endl << endl;
 //cerr << "where we stop "  << child->Value() << endl;
			regex dateRegex("([A ?-Z ?]+),( [0-9]{4})");
			if (regex_search(biginfo, match, dateRegex)) {
				//cerr << "DATE Regex Matched: " << match[1].str() << match[2].str() << endl ;
			 article[counter].date = capitalize(trim(match[1].str())) + match[2].str();                         //if regex is matched then capitalize, trim and concatenate the month and year
			}

			regex volRegex("([VYM] ?[Oo] ?[LIR]v?[,.] [IVXLCivxlc0-9]+)");                                         //if regex is matched then trim volume, if volume is not "VOL" then find and replace with "VOL" and fix wonky roman numerals
			if (regex_search(biginfo, match, volRegex)) {
				//cerr << "VOL Regex Matched: " << match[1].str() << endl ;
			 article[counter].volume = trim(match[1].str());
				findAndReplaceAll(article[counter].volume, "VOIv", "VOL");
			 article[counter].volume = capitalize(article[counter].volume);
			 article[counter].volume = fixRomanNumerals(article[counter].volume);
			}

			regex numberRegex("(N ?[oO] ?[.,]? [IVXLCivxlc0-9]+)");                                          //if regex is matched then capitalize and trim the "number" and fix roman numerals if incorrect
			if (regex_search(biginfo, match, numberRegex)) {
				//cerr << "NUM Regex Matched: " << match[1].str() << endl ;
			 article[counter].number = capitalize(trim(match[1].str()));
			 article[counter].number = fixRomanNumerals(article[counter].number);
			}

			//cerr << "BOOK INFO : " << article[counter].number << " | " << article[counter].date << " | " << article[counter].volume << endl;
		}


		if (isArticle == true)  //only increase counter if articles are found
			counter++;

	}
	 
	// cout << "HERE" << endl;
}

 /*
 
 findAuthor(const string &hold) has if statements to look at regular expressions. this is to find the author using "by"
 
 
  */
string findAuthor(const string &hold) {    //to find authors, use reg expression

	//cerr << "Looking at: ---" << hold << "---" << endl;
	try {
		regex r("B[yv] ([a-z.,-^ ]*?) ?(,|\\.$).*?$");
		regex r2("B[yv] ([a-z.,-^ ]*?)[ .,]?[\n\r]+");
		regex r3("B[yv] ([a-z.,-^ ]*?)[\n\r]+");
		
		smatch match;

		if (regex_search(hold, match, r)) {
	//		cerr << "Found R: " << match[1] << endl;
			return match[1];
		} 
		else if (regex_search(hold, match, r2)) {
	//		cerr << "Found R2: " << match[1] << endl;
			return match[1];
		} 
		else {
			//cerr << "Found Nothign " << endl;
			return string("");
	//		 cerr << " not Working: " << hold << endl;
		}

	} catch (regex_error& e) {
		return string("");
		 cerr << " error!" << string("") << endl;
	}

	return string("");
}
/*
	
print() prints out all of the article metadata. if there are blanks in the volume, date, issue or number, it just copies from the previous entries until one is ecountered	
 */
void print() {
	// cout << "Title" << "\t" <<endl;
    // 	<< "Author" << "\t" << "Date" << "\t" 
	//     << "Volume" << "\t" << "Number" << "\t" << "Issue" << "\t" 
	// 	<< "StartPageID" << "\t" << "EndPageID" << "\t" 
	// 	<< "StartPage" << "\t" << "EndPage" <<  endl;

//cout << "HERE1" << endl;
	for (int c = 0; c < counter; c++)
	{
//cout << "Article c: " << article[c] << endl;
// cout << "Article c-1: " << article[c - 1] << endl;

		if (article[c].date == "" && c>0)
		{
		 article[c].date = article[c - 1].date;
		 article[c].issue = article[c - 1].issue;
		 article[c].number = article[c - 1].number;
		}

		if (article[c].volume == "" && c>0)
	 article[c].volume = article[c - 1].volume;

		cout << article[c].header << "\t" 
		     << article[c].author << "\t" << article[c].date << "\t" 
			 << article[c].volume << "\t" << article[c].number << "\t" << article[c].issue << "\t" 
			 << article[c].startPageID << "\t" << article[c].endPageID << "\t" 
			 << article[c].startPage << "\t" << article[c].endPage <<  endl;

	}

}
 
/*

trim(string str) trims away new line and extra blank characters in xml

 */
string trim(string str)     
{
	try {

		regex rLeft("^[\r\n .]+");
		str = regex_replace(str, rLeft, "");    //if there are spaces before the first word

		regex rRight("[\r\n .]+$");
		str = regex_replace(str, rRight, "");   // if there are spaces after the last word

		regex rNewline("[\r\n]+");
		str = regex_replace(str, rNewline, " ");   //if there is a newline at end of string, replace with a space

		return str;
	} catch (exception& e) {
		cerr << "exception caught: " << e.what() << " trying to parse " << str << "\n"; 
		return "";
	}
	
}
/*

capitalize(string str) capitalizes the first letter of each word and puts every other letter in lowercase

 */
string capitalize(string str)
{
	
	try {
		if (!str.empty())
		{
			str[0] = toupper(str[0]);     //capitalizes first letter

			for (size_t counter = 1; counter < str.length(); ++counter)     //at second letter on until next word, replaces letters with lower case ones
			{
				str[counter] = tolower(str[counter]);
				if (str[counter - 1] == ' ' || str[counter-1] == '-' || str[counter-1] =='.')  //if there is a period or space before the letter, it is a new word and capitalize it
					str[counter] = toupper(str[counter]);
			}
		}
	} catch (exception& e) {
		cerr << "exception caught: " << e.what() << "\n"; 
		return "";
	}

	return str;
}

/*

findAndReplaceAll(string & data, string toSearch, string replaceStr)   looks for the occurence of a word and replaces it

 */
void findAndReplaceAll(string & data, string toSearch, string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);      
 
	// Repeat till end is reached
	while( pos != string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}


/*


fixRomanNumerals(string str) looks at letters that would be in roman numerals, and if they are lowercase, capitalizes them

 */
string fixRomanNumerals(string str) {
	smatch match;

	regex r("([IVXLCivxlc]{2,})");
	if (regex_search(str, match, r)) {
		string u = match[1].str();

		for (size_t counter = 1; counter < u.length(); ++counter) {
			u[counter] = toupper(u[counter]);     //capitalizes each letter and then replaces them
		}
		str = regex_replace(str, r, u);
	}

	return str;
}