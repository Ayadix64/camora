#ifndef  ACONFIG
#define  ACONFIG
#include <algorithm>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>



static int readConf(std::string file , std::string conf , std::string& readTo){
	std::ifstream ifile(file);
	std::string str;
	if(ifile.is_open()){
		for ( int i = 0 ; std::getline(ifile,str) ; i++) {
			if(!str.size())continue;

			int semi = str.find(":");
			int hash = str.find("#");
			int textStart = 0;
			int textEnd =  0;
			int startOfline = 0;

			for(; (str[startOfline] == ' ' || str[startOfline] =='\t') && startOfline<str.size();startOfline++);;

			if(startOfline == str.size())continue;
			if(semi == -1)continue;
			if(hash != -1 && hash < semi )continue;

			std::string co = str.substr(startOfline,semi);
			if(co==conf){
				for(int ii = semi, chek = 0 ; ii<str.size(); ii++){
					if(str[ii] == '\"'){
						if(chek == 0){

								textStart = ii+1;
								chek++;

						}else if(chek == 1){
							textEnd = ii ;
							break;
						}
					}
				}
				if(textStart > semi && textStart && textEnd && textEnd > textStart){
					readTo = str.substr(textStart , textEnd - textStart );
					ifile.close();
					return 0;
				}
			}

		}
		ifile.close();

	}else {
		std::cout << "\n[ERROR]: never finde config file \""<<file <<".";
		return 1;
	}
	return 1;
}


static int writeConf(std::string file , std::string conf , std::string valu){
	std::ifstream ifi(file);
	std::string str;
	std::string fullCf;
	if(ifi.is_open()){
		bool isFond = false;
		for ( int i = 0 ; std::getline(ifi,str) ; i++) {
			if(!str.size())continue;

			int semi = str.find(":");
			int hash = str.find("#");
			int textStart = 0;
			int textEnd =  0;
			int startOfline = 0;

			for(; (str[startOfline] == ' ' || str[startOfline] =='\t') && startOfline<str.size();startOfline++);;

			if(startOfline == str.size())continue;
			if(semi == -1)continue;
			if(hash != -1 && hash < semi )continue;

			std::string co = str.substr(startOfline,semi);
			if(co==conf){
				fullCf.append(conf);
				fullCf.append(":");
				fullCf.append("\"");
				fullCf.append(valu);
				fullCf.append("\"\n");
				isFond = true;

			}else {
				fullCf.append(str);
				fullCf.append("\n");
			}

		}
		std::ofstream ofi(file);
		if(isFond){
			ofi<<fullCf;
		}else {
			fullCf.append(conf);
			fullCf.append(":");
			fullCf.append("\"");
			fullCf.append(valu);
			fullCf.append("\"\n");
			ofi<<fullCf;
		}
		if(!ofi.is_open())return 1;
		//std::cout << fullCf;
		ifi.close();
		return 0;
	}
	else{
		std::ofstream ofi(file);
		fullCf.append(conf);
		fullCf.append(":");
		fullCf.append("\"");
		fullCf.append(valu);
		fullCf.append("\"\n");
		ofi<<fullCf;
		ofi.close();
	}
	return 0;

}


static bool IsTherConfig(std::string file, std::string conf){
	std::ifstream ifile(file);
	std::string str;
	if(ifile.is_open()){
		for ( int i = 0 ; std::getline(ifile,str) ; i++) {
			if(!str.size())continue;

			int semi = str.find(":");
			int hash = str.find("#");
			int textStart = 0;
			int textEnd =  0;
			int startOfline = 0;

			for(; (str[startOfline] == ' ' || str[startOfline] =='\t') && startOfline<str.size();startOfline++);;

			if(startOfline == str.size())continue;
			if(semi == -1)continue;
			if(hash != -1 && hash < semi )continue;

			std::string co = str.substr(startOfline,semi);
			if(co==conf){return true;}
		}

		ifile.close();
		return false;
	}else {
		std::cout << "\n[ERROR]: never finde config file \""<<file <<".";
		return false;
	}
	return false;
}



/* MIT License                                                                       *
 *
 * Copyright (c) 2026 Ayadi                                                          *

 * Permission is hereby granted, free of charge, to any person obtaining a copy      *
 * of this software and associated documentation files (the "Software"), to deal     *
 * in the Software without restriction, including without limitation the rights      *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell         *
 * copies of the Software, and to permit persons to whom the Software is             *
 * furnished to do so, subject to the following conditions:                          *

 * The above copyright notice and this permission notice shall be included in all    *
 * copies or substantial portions of the Software.                                   *

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER            *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,     *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     *
 * SOFTWARE.                                                                         */


#endif
