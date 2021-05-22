#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stdio.h>
#include <string.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#define MAX 1000
using namespace std;


/* 
	this function deals with url
	if what it deals with is a url
	it will return 0
	if what it deals with is not a url
	it will return from 1 to 5
	if what it deals with is not a url and onUrl gets a '\n'
    it will return 2 or 4
*/
int onUrl(string infile,string outfile) {
	
	ifstream in(infile);
	if (!in.is_open()){
    	cout << "Error opening file"; 
		exit(1);
	}
	ofstream out(outfile);
	if (!out.is_open()){
    	cout << "Error opening file"; 
		exit(1);
	}
	
	int i = 0;
	char ch, name[MAX], url[MAX];

	while (!in.eof()){
		in.get(ch);
		if (ch == ']' || ch == '\n' || i > 999) {
			break;
		}
		name[i++] = ch;
	}
	name[i] = '\0';
	if (ch != ']') {
		out << "[" << name << ch;
		out.close();
		in.close();
		return 1;
	}
	
	in.get(ch);
	while(ch  == ' ') 
		in.get(ch);

	if(ch != '(') {
		if(ch == '\n'){
			out << "[" << name << "]";
			out.close();
			in.close();
			return 2;
		}
		else{
			out << "[" << name << "]" << ch;
			out.close();
			in.close();	
			return 3;
		}
	}

	i = 0;
	while(!in.eof()){
		in.get(ch);
		if (ch == ')' || ch == '\n' || i > 999) {
			break;
		}
		url[i++] = ch;
	}
	url[i] = '\0';

	if(ch != ')'){
		if(ch == '\n'){
			out << "[" << name << "]" << "(" << url; 
			out.close();
			in.close();
			return 4;
		}
		else{
			out << "[" << name << "]" << "(" << url << ch; 
			out.close();
			in.close();
			return 5;
		}
	}

	out << "<a href=\"" << url << "\"" << ">" << name <<"</a>";	
	out.close();
	in.close();
	return 0;
}
