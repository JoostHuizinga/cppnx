/*
 * CX_CppnType.hpp
 *
 *  Created on: Feb 12, 2019
 *      Author: joost
 */

#ifndef CPPN_X_INCLUDE_CX_CPPNTYPE_HPP_
#define CPPN_X_INCLUDE_CX_CPPNTYPE_HPP_

#include "CE_Defines.h"
#include "CX_Debug.hpp"

class Cppn;

class CppnType {
public:
	virtual ~CppnType(){};
	virtual int getNrInputNodes() = 0;
	virtual int getCoordsPerNode() = 0;
	virtual double getInputValue(int node, int index) = 0;
	virtual CppnType* clone() const = 0;
	virtual bool inputIsUsed(std::string xml_label) = 0;
	virtual int getInputIndex(std::string xml_label) = 0;
	virtual int getImageWidth() = 0;
	virtual int getImageHeight() = 0;
	virtual void processValue(QSharedPointer<QImage> pixels, const size_t& index, const double& value) = 0;
	virtual Cppn* getNewCppn() = 0;
	void claim(Cppn* cppn){
		if(_owned_by != 0){
			throw std::runtime_error("Info claimed more than once!");
		}
		_owned_by = cppn;
	}
protected:
	Cppn* _owned_by = 0;
};

class TwoDimCppnType: public CppnType{
public:
	TwoDimCppnType(int min_x=-1, int max_x=1, int min_y=-1, int max_y=1,
			int width=IMAGE_WIDTH, int height=IMAGE_HEIGHT)
	{
	    _min_x = min_x;
	    _max_x = max_x;
	    _min_y = min_y;
	    _max_y = max_y;
	    _width = width;
	    _height = height;
	    _initCache();
	}

	TwoDimCppnType(const TwoDimCppnType& other){
	    _min_x = other._min_x;
	    _max_x = other._max_x;
	    _min_y = other._min_y;
	    _max_y = other._max_y;
	    _width = other._width;
	    _height = other._height;
	    _cached_values = other._cached_values;
	    // Explicitly do not copy ownership
	    _owned_by = 0;
	}

	int getNrInputNodes(){return 4;}
	int getCoordsPerNode(){return _width*_height;}
	int getImageWidth(){return _width;}
	int getImageHeight(){return _height;}
	double getInputValue(int node, int index){return _cached_values[node][index];}
	CppnType* clone() const{return new TwoDimCppnType(*this);}
	bool inputIsUsed(std::string xml_label);
	void processValue(QSharedPointer<QImage> pixels, const size_t& index, const double& value);
	int getInputIndex(std::string xml_label);
	Cppn* getNewCppn();

private:
	void _initCache();

	const int _input_x_index = 0;
	const int _input_y_index = 1;
	const int _input_d_index = 2;
	const int _input_b_index = 3;

    int _min_x;
    int _max_x;
    int _min_y;
    int _max_y;
    int _width;
    int _height;

    std::vector< std::vector<double> > _cached_values;
};


class OneDimCppnType: public CppnType{
public:
	OneDimCppnType(int min_x=-1, int max_x=1, int width=IMAGE_WIDTH){
	    _min_x = min_x;
	    _max_x = max_x;
	    _width = width;
	    _initCache();
	}

	OneDimCppnType(const OneDimCppnType& other){
	    _min_x = other._min_x;
	    _max_x = other._max_x;
	    _width = other._width;
	    _cached_values = other._cached_values;
	    // Explicitly do not copy ownership
	    _owned_by = 0;
	}

	int getNrInputNodes(){return 2;}
	int getCoordsPerNode(){return _width;}
	int getImageWidth(){return _width;}
	// TODO: Should be a parameter that you can set
	int getImageHeight(){return 256;}
	double getInputValue(int node, int index){return _cached_values[node][index];}
	CppnType* clone() const{return new OneDimCppnType(*this);}
	bool inputIsUsed(std::string xml_label);
	void processValue(QSharedPointer<QImage> pixels, const size_t& index, const double& value);
	int getInputIndex(std::string xml_label);
	Cppn* getNewCppn();

private:
	void _initCache();

	const int _input_x_index = 0;
	const int _input_b_index = 1;

    int _min_x;
    int _max_x;
    int _width;
    std::vector< std::vector<double> > _cached_values;
};



#endif /* CPPN_X_INCLUDE_CX_CPPNTYPE_HPP_ */
