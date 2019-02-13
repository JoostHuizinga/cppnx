/*
 * CX_CppnType.cpp
 *
 *  Created on: Feb 12, 2019
 *      Author: joost
 */

#include <QSharedPointer>
#include <QImage>

#include "CX_CppnType.hpp"
#include "CE_Cppn.h"
#include "CE_ActivationFunctions.h"


Cppn* TwoDimCppnType::getNewCppn(){
	TwoDimCppnType* new_cppn_info = new TwoDimCppnType(*this);
	Cppn* cppn = new Cppn(new_cppn_info);

	id_t id = 0;
	Node* in_x = new Node("0", util::toString(id++), XML_TYPE_INPUT, XML_LINEAR, INPUT_X);
	Node* in_y = new Node("0", util::toString(id++), XML_TYPE_INPUT, XML_LINEAR, INPUT_Y);
	Node* in_d = new Node("0", util::toString(id++), XML_TYPE_INPUT, XML_LINEAR, INPUT_D);
	Node* in_b = new Node("0", util::toString(id++), XML_TYPE_INPUT, XML_LINEAR, INPUT_BIAS);
	Node* out_ink = new Node("0", util::toString(id++), XML_TYPE_OUTPUT, XML_LINEAR, OUTPUT_INK);

	cppn->addNode(in_x);
	cppn->addNode(in_y);
	cppn->addNode(in_d);
	cppn->addNode(in_b);
	cppn->addNode(out_ink);

	cppn->addConnection(new Edge("0", util::toString(id++), in_x, out_ink));
	cppn->addConnection(new Edge("0", util::toString(id++), in_y, out_ink));
	cppn->addConnection(new Edge("0", util::toString(id++), in_d, out_ink));
	cppn->addConnection(new Edge("0", util::toString(id++), in_b, out_ink));

	return cppn;
}

void TwoDimCppnType::processValue(QSharedPointer<QImage> pixels, const size_t& index, const double& value){
    dbg::trace trace("cppn", DBG_HERE);
	size_t localindex = index*4;
	//Grey does not use the min() function to prevent a bug on windows.
	unsigned char red = 255;
	if(std::abs(value) < 1.0) red = std::abs(value)*255;
	unsigned char green(std::min(std::max(value, 0.0), 1.0)*255);
	unsigned char blue(std::min(std::max(value, 0.0), 1.0)*255);
	if(value > 1.0) blue = (1-std::min(std::max(((std::abs(value)-1)/5), 0.0), 1.0))*255;
	if(value < -1.0) blue = std::min(std::max(((std::abs(value)-1)/5), 0.0), 1.0)*255;
	pixels->bits()[localindex]=blue;
	pixels->bits()[localindex+1]=green;
	pixels->bits()[localindex+2]=red;
}

bool TwoDimCppnType::inputIsUsed(std::string xml_label){
	if(xml_label == INPUT_X){
		return true;
	} else if(xml_label == INPUT_Y){
		return true;
	} else if(xml_label == INPUT_D){
		return true;
	} else if(xml_label == INPUT_BIAS){
		return true;
	} else{
		return false;
	}
}

int TwoDimCppnType::getInputIndex(std::string xml_label){
	if(xml_label == INPUT_X){
		return _input_x_index;
	} else if(xml_label == INPUT_Y){
		return _input_y_index;
	} else if(xml_label == INPUT_D){
		return _input_d_index;
	} else if(xml_label == INPUT_BIAS){
		return _input_b_index;
	} else{
		throw std::invalid_argument("Unknown XML label: " + xml_label);
	}
}

void TwoDimCppnType::_initCache(){
	_cached_values.resize(getNrInputNodes());
	for(int i=0; i<getNrInputNodes(); ++i){
		_cached_values[i].resize(getCoordsPerNode());
	}
	for(int x=0; x<_width; x++){
		for(int y=0; y<_height; y++){
			int x_diff = _max_x - _min_x;
			int y_diff = _max_y - _min_y;
			double xv = (double(x)/(double(_width)/x_diff) + _min_x);
			double yv = (double(y)/(double(_height)/y_diff) + _min_y);
			size_t index = x + y*_width;
			_cached_values[_input_x_index][index] = xv;
			_cached_values[_input_y_index][index] = yv;
			_cached_values[_input_d_index][index] = double(std::sqrt(float(xv*xv+yv*yv))*1.4);
			_cached_values[_input_b_index][index] = 1.0;
		}
	}
}

Cppn* OneDimCppnType::getNewCppn(){
	OneDimCppnType* new_cppn_info = new OneDimCppnType(*this);
	Cppn* cppn = new Cppn(new_cppn_info);

	id_t id = 0;
	Node* in_x = new Node("0", util::toString(id++), XML_TYPE_INPUT, XML_LINEAR, INPUT_X);
	Node* in_b = new Node("0", util::toString(id++), XML_TYPE_INPUT, XML_LINEAR, INPUT_BIAS);
	Node* out_ink = new Node("0", util::toString(id++), XML_TYPE_OUTPUT, XML_LINEAR, OUTPUT_INK);

	cppn->addNode(in_x);
	cppn->addNode(in_b);
	cppn->addNode(out_ink);

	cppn->addConnection(new Edge("0", util::toString(id++), in_x, out_ink));
	cppn->addConnection(new Edge("0", util::toString(id++), in_b, out_ink));

	return cppn;
}

void OneDimCppnType::processValue(QSharedPointer<QImage> pixels, const size_t& index, const double& value){
    dbg::trace trace("cppn", DBG_HERE);
    int desired_y = 0;
    float y_offset = pixels->height()/2.0;
    float y_scale = y_offset*0.9;

    if (index==0) pixels->fill(QColor(101, 151, 74).rgb());

    int width = pixels->width();
    int x = index % width;
    int y = index / width;

    if (y!=desired_y) return;

    int local_value = (int)((value*y_scale + y_offset));
    for(int i=0; i<=local_value && i<pixels->height(); ++i){
    	pixels->setPixel(x, i, QColor(234, 234, 255).rgb());
    }
}

bool OneDimCppnType::inputIsUsed(std::string xml_label){
	if(xml_label == INPUT_X){
		return true;
	} else if(xml_label == INPUT_BIAS){
		return true;
	} else{
		return false;
	}
}

int OneDimCppnType::getInputIndex(std::string xml_label){
	if(xml_label == INPUT_X){
		return _input_x_index;
	} else if(xml_label == INPUT_BIAS){
		return _input_b_index;
	} else{
		throw std::invalid_argument("Unknown XML label: " + xml_label);
	}
}

void OneDimCppnType::_initCache(){
	_cached_values.resize(getNrInputNodes());
	for(int i=0; i<getNrInputNodes(); ++i){
		_cached_values[i].resize(getCoordsPerNode());
	}
	for(int x=0; x<_width; x++){
			int x_diff = _max_x - _min_x;
			double xv = (double(x)/(double(_width)/x_diff) + _min_x);
			size_t index = x;
			_cached_values[_input_x_index][index] = xv;
			_cached_values[_input_b_index][index] = 1.0;
	}
}
