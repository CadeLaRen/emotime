#include "matrix_io.h"
#include <algorithm>

using namespace std;
using namespace cv;


std::string matrix_io_fileExt( std::string & file){
  return file.substr(file.find_last_of(".") + 1); 
}
std::string matrix_io_fileName( std::string & file){
  return file.substr( std::max((int)file.find_last_of(PATH_SEPARATOR),0) , file.find_last_of(".") ); 
}

cv::Mat matrix_io_load( std::string & filePath){
  try {
		string file = filePath;
    string format =  matrix_io_fileExt(file); 
    if(format==XMLEXT || format==YMLEXT) {
       string name = matrix_io_fileName(file);  
       FileStorage fs(file, FileStorage::READ);
       Mat * mat = new Mat();
       fs[name] >> *mat;
       fs.release();
       return *mat;
    } else {
      // Otherwise threat it as image
      return imread( filePath, CV_LOAD_IMAGE_GRAYSCALE );
    }
  }
	catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return  *(new Mat(0,0,CV_32FC1));
	}

}

bool matrix_io_save( cv::Mat & mat, std::string & filePath){
  try {
		string file = filePath;
    string format =  matrix_io_fileExt(file); 
    if(format==XMLEXT || format==YMLEXT) {
       string name = matrix_io_fileName(file);  
       FileStorage fs(file, FileStorage::WRITE);
       fs << name << mat;
       fs.release();
    } else {
      // Otherwise threat it as image
		  imwrite( file, mat );
    }
    return true;
  }
	catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return false;
	}

}
