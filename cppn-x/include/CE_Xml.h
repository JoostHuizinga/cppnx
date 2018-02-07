/*
 * CE_Xml.h
 *
 *  Created on: Jun 11, 2013
 *      Author: joost
 */

#ifndef CE_XML_H_
#define CE_XML_H_

#include <queue>
#include <string>
#include "CE_Util.h"

namespace ce_xml{
typedef std::vector<std::string> vec_str_t;

const std::string	any= (".*");
const std::string	first_line = ("xml version encoding standalone");
const vec_str_t first_line_v = {"xml", "version", "encoding", "standalone"};
const std::string	ionode= "node label type";
const vec_str_t ionode_v = {"node", "label", "type"};
const std::string	node="node type";
const vec_str_t	node_v={"node", "type"};
const std::string	colornode= ("node affinity bias type");
const vec_str_t	colornode_v= {"node","affinity","bias","type"};
const std::string	iocolornode= ("node affinity bias label type");
const vec_str_t	iocolornode_v= {"node","affinity","bias","label","type"};
const std::string	link= ("link");
const vec_str_t	link_v= {"link"};
const std::string	marking= ("marking branch id");
const vec_str_t	marking_v= {"marking", "branch", "id"};
const std::string	activation= ("activation");
const vec_str_t	activation_v= {"activation"};
const std::string	color= ("color r g b");
const vec_str_t	color_v= {"color","r","g","b"};
//const std::string	color_old= ("color");
const std::string	position= ("position x y");
const vec_str_t	position_v= {"position","x","y"};
const std::string draw_order= ("draw-order z-value");
const vec_str_t	draw_order_v= {"draw-order","z-value"};
const std::string	source= ("source branch id");
const vec_str_t	source_v= {"source","branch","id"};
const std::string	target= ("target branch id");
const vec_str_t	target_v= {"target","branch","id"};
const std::string	weight= ("weight");
const vec_str_t	weight_v= {"weight"};
const std::string	original_weight= ("original-weight");
const vec_str_t	original_weight_v= {"original-weight"};

const std::string	cppn_data= ("cppn-data version");
const vec_str_t	cppn_data_v= {"cppn-data","version"};
const std::string	nodes_count= ("nodes count");
const vec_str_t	nodes_count_v= {"nodes","count"};
const std::string	link_count= ("links count");
const vec_str_t	link_count_v= {"links","count"};
const std::string	buttons_count= ("buttons count");
const vec_str_t	buttons_count_v= {"buttons","count"};
const std::string	color_button= ("colorButton");
const vec_str_t	color_button_v= {"colorButton"};
const std::string	text= ("text");
const vec_str_t	text_v= {"text"};
const std::string	count= (".*count=\"(.*)\".*");
const std::string	data=("data version");
const vec_str_t	data_v={"data","version"};
const std::string	storage= ("storage maximum minimum name");
const vec_str_t	storage_v= {"storage","maximum","minimum","name"};
const std::string	generation= ("generation number size");
const vec_str_t	generation_v= {"generation","number","size"};
const std::string	genome= ("genome age");
const vec_str_t	genome_v= {"genome","age"};
const std::string	genomePhen= ("genome age phenotype");
const vec_str_t	genomePhen_v= {"genome","age","phenotype"};
const std::string	identifier= ("identifier branch id");
const vec_str_t	identifier_v= {"identifier","branch","id"};
const std::string	parent_count= ("parents count");
const vec_str_t	parent_count_v= {"parents","count"};
const std::string	color_label= ("color-label id");
const vec_str_t	color_label_v= {"color-label","id"};

const std::string	bookends= ("bookends start end step");
const vec_str_t	bookends_v= {"bookends","start","end","step"};

const std::string	nodeview= ("nodeview");
const vec_str_t	nodeview_v= {"nodeview"};
const std::string	nodeviews_count= ("nodeviews count");
const vec_str_t	nodeviews_count_v= {"nodeviews","count"};

//main.zip
const std::string	parameters= ("parameters");
const vec_str_t	parameters_v= {"parameters"};
const std::string	series= ("series branch");
const vec_str_t	series_v= {"series","branch"};
const std::string	branchFrom= ("branchFrom branch id");
const vec_str_t	branchFrom_v= {"branchFrom","branch","id"};
const std::string	branchFromEmpty= ("branchFrom");
const vec_str_t	branchFromEmpty_v= {"branchFrom"};
const std::string	generation_storage= ("generation number storage");
const vec_str_t	generation_storage_v= {"generation","number","storage"};

inline std::queue<std::string> split(std::string str){
	std::queue<std::string> result;
	std::size_t found = 0;
	std::size_t previous_found = 0;
	do {
		found = str.find(" ", previous_found);
		result.push(str.substr(previous_found, found - previous_found));
		previous_found = found+1;
	}while (found!=std::string::npos);

	return result;
}


inline std::string appendParameters(std::string result, std::string templateStr, std::queue<std::string> parameters){
	std::queue<std::string> parts = split(templateStr);
	result+=parts.front();
	parts.pop();

	while(!parts.empty()){
		result+= " " + parts.front();
		parts.pop();
		result+= "=\"" + parameters.front() + "\"";
		if(parameters.size() > 1) parameters.pop();
	}
	return result;
}

inline std::string getFirstLine(){
	std::string result = "<?";
	std::queue<std::string> parameters;
	parameters.push("1.0");
	parameters.push("UTF-8");
	parameters.push("no");
	result = appendParameters(result, first_line, parameters);
	result+="?>";
	return result;
}

inline std::string getOpenXmlString(std::string templateStr, std::queue<std::string> parameters){
	std::string result = "<";
	result = appendParameters(result, templateStr, parameters);
	result+=">";
	return result;
}

inline std::string getOpenXmlString(std::string templateStr, std::string string1 = "\"", std::string string2 = "\"", std::string string3 = "\"", std::string string4 = "\""){
	std::queue<std::string> parameters;
	parameters.push(string1);
	if(string2 != "\"") parameters.push(string2);
	if(string3 != "\"") parameters.push(string3);
	if(string4 != "\"") parameters.push(string4);
	return getOpenXmlString(templateStr, parameters);
}

inline std::string getOpenXmlString(std::string templateStr, size_t number){
	std::queue<std::string> parameters;
	parameters.push(util::toString(number));
	return getOpenXmlString(templateStr, parameters);
}

inline std::string getCloseXmlString(std::string templateStr){
	std::queue<std::string> parts = split(templateStr);
	std::string result = "</";
	result+=parts.front();
	result+=">";
	return result;
}

inline std::string getOneLineXmlString(std::string templateStr, std::queue<std::string> parameters){
	std::string result = "<";
	result = appendParameters(result, templateStr, parameters);
	result+="/>";
	return result;
}

inline std::string getOneLineXmlString(std::string templateStr, std::string string1 = "\"", std::string string2 = "\"", std::string string3 = "\"", std::string string4 = "\""){
	std::queue<std::string> parameters;
	parameters.push(string1);
	if(string2 != "\"") parameters.push(string2);
	if(string3 != "\"")parameters.push(string3);
	if(string4 != "\"")parameters.push(string4);
	return getOneLineXmlString(templateStr, parameters);
}

inline std::string getOpenAndCloseXmlString(std::string templateStr, std::string filler){
	std::string result = getOpenXmlString(templateStr);
	result+=filler;
	result+=getCloseXmlString(templateStr);
	return result;
}


}

#endif /* CE_XML_H_ */
