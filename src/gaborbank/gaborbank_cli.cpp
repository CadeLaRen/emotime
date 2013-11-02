/**
 *
 * */
#include <iostream>
#include <sstream>
#include <string>

#include "gaborbank.h"
#include "../adaboost/featselect.h"
#include "../utils/matrix_io.h"

using namespace std;
using namespace cv;


void help(){
	cout << "Usage:" << endl;
	cout << "   gaborbank_cli <imageWidth> <imageHeight> <inputImage> <outputFile> [<filterFile>]" << endl;
	cout << "Parameters:" << endl;
	cout << "   <imageWidth>    - Width of the image, the input image will be scaled" << endl;
	cout << "   <imageHeight>   - Height of the image, the input image will be scaled" << endl;
	cout << "   <inputImage>    - Input image" << endl;
	cout << "   <outputFile>    - Output file  where to store filtered images" << endl;
	cout << "   <filterFile>    - Text file containing the list of pixel to keep" << endl;
	cout << endl;
}
void banner(){
 	cout << "GaborBank Utility:" << endl;
 	cout << "     Filter with a bank of Gabor filters with different "<< endl;
 	cout << "     orientations and frequencies. (Gabor magnitude)" << endl;
}

int main( int argc, const char* argv[] )
{
	
	if (argc < 5) {
		banner();
		help();
		cerr << "ERR: missing parameters" << endl;
		return -3;
	} 
	try {
		unsigned int width = abs(atoi(argv[1]));
		unsigned int height = abs(atoi(argv[2]));
		string infile = string(argv[3]);
		string outfile = string(argv[4]);
    bool filter=false;
    const char * filterfile;
    
    if (argc == 6){
      filter=true;
      filterfile=argv[5];
      cout << "INFO: filtering with filter at "<< filterfile << endl;
    }
	
		Mat img = matrix_io_load( infile );
		Mat scaled;
		resize( img, scaled, Size(width,height), 0, 0, CV_INTER_AREA );

		vector<GaborKern*> bank;
		gaborbank_getGaborBank(bank);
    cout << "INFO: filtering with "<< bank.size() << " filters" << endl;
		Mat dest = gaborbank_filterImage(scaled, bank);

    if (filter){
      set<unsigned int> selected=featselect_load(filterfile); 
      dest = featselect_select(dest,selected);
    }

    cout << "INFO: saving to " << outfile << endl; 
    matrix_io_save( dest, outfile );

    dest.release();
	}
	catch (int e) {
		cerr << "ERR: Exception #" << e << endl;
		return -e;
	}
}
