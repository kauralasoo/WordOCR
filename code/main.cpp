#include <iostream> 
 
#include <cv.h> 
#include <cxcore.h> 
#include <highgui.h> 
 
using namespace std;

vector<int> findY(IplImage* src_img){
	int width = src_img-> width;
	CvMat data;
	vector<int> vec; 
	CvScalar val = cvRealScalar(0);
	CvScalar min = cvRealScalar(0);
	cvGetCol(src_img, &data, 1);
	min = cvSum(&data);
	for (int i = 0; i < width; i++){
		cvGetCol(src_img,&data, i);
		val = cvSum(&data);
		//cout << val.val[0] << endl;
		if (val.val[0] >= min.val[0]){ 
		 vec.push_back(i);
		}
	}
	return(vec);
}

vector<int> findX(IplImage* src_img){
	int height = src_img-> height;
	CvMat data;
	vector<int> vec; 
	CvScalar val = cvRealScalar(0);
	CvScalar min = cvRealScalar(0);
	cvGetRow(src_img, &data, 0);
	min = cvSum(&data);
	for (int i = 0; i < height; i++){
		cvGetRow(src_img,&data, i);
		val = cvSum(&data);
		//cout << val.val[0] << endl;
		if (val.val[0] >= min.val[0]){ 
			vec.push_back(i);
		}
	}
	return(vec);
}

vector <int> findCutoffs(vector<int> vec){
	vector <int> results;
	for (unsigned int i = 0; i < vec.size()-1; i++){
		int a = vec.at(i);
		int b = vec.at(i+1);
		if (b - a > 1){
			results.push_back(a);
			results.push_back(b);
		}
	}
	return(results);
}

void extractLetters(IplImage* src_img, vector<int> y_cutoff, string file_name){
	int count = 0;
	for(unsigned int i = 0; i < y_cutoff.size(); i = i+2){
		count++;
		//Create path to write the pictures:
		string path = "pictures/";
		size_t pos1 = file_name.find("/");
		string word = file_name.substr(pos1+1);
		size_t pos2 = word.find(".");
		string word1 = word.substr(0,pos2);
		path.append(word1);
		path.append("_");
		stringstream os;
		os << i;
		string str_os = os.str();
		path.append(str_os);
		string path_pic = path;
		path.append(".xml");
		path_pic.append(".png");
		const char* p = path.c_str();
		const char* p_pic = path_pic.c_str();
	
		

		//Cut vertically
		int y1 = y_cutoff.at(i);
		int y2 = y_cutoff.at(i+1);
		//cout << y1 << y2 << endl;
		int new_width = y2-y1;
		int new_height = src_img -> height;
		cvSetImageROI(src_img, cvRect(y1,0,new_width, new_height));
		IplImage *img_hcut = cvCreateImage(cvGetSize(src_img),
                               src_img->depth,
                               src_img->nChannels);
		cvCopy(src_img, img_hcut, NULL);

		//Cut horizontally
		vector<int> x_vec = findX(img_hcut);
		vector<int> x_cutoff = findCutoffs(x_vec);
		int x1 = 0;
		int x2 = 0;
		if (x_cutoff.size() == 2){
			x1 = x_cutoff.at(0);
			x2 = x_cutoff.at(1);
		}
		else{
			x1 = x_cutoff.at(0);
			x2 = x_cutoff.at(x_cutoff.size()-1);
		}
		int h_height = x2-x1;
		int h_width = img_hcut -> width;
		cvSetImageROI(img_hcut, cvRect(0,x1,h_width, h_height));
		//cout << x1 << " " << x2 << endl;

		//Calculate features
		double aspect_ratio = h_width*1.0/h_height;
		string current_letter = word1.substr((count-1),1);
		string comment = "Aspect ratio: ";
		stringstream cs;
		cs << aspect_ratio << "\n" << "Letter: " << current_letter;
		string str_cs = cs.str();
		comment.append(str_cs);
		const char* c = comment.c_str();


		cout << aspect_ratio << " " << h_height/16 << " " << h_width/8 << endl;

		IplImage *scaledResult = cvCreateImage( cvSize( 8, 16 ), 8, 1 );
		cvResize(img_hcut, scaledResult);
		
		cvSave(p, scaledResult,"letter",c);
		cvSaveImage(p_pic, scaledResult);
		cvReleaseImage( &scaledResult );
		cvReleaseImage( &img_hcut );
		cvResetImageROI(src_img);
	}
}

 
int main(int argc, char* argv[]) 
{ 
	//Load image
	string file_name = argv[1];
	const char* f = file_name.c_str();
	IplImage *img = cvLoadImage(f); 
	int height = img->height;
	int width = img->width;

 	//Convert image to grayscale:
 	IplImage* out = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1); 
 	cvCvtColor (img, out,CV_BGR2GRAY); // Convert image to grayscale
 	cvAdaptiveThreshold(out,out,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,101,5); // Convert image to binary using adaptive thresholding
 
	//Clone image
	IplImage* bb_img;
 	bb_img = cvCloneImage(out);


	//Find vertical cutoffs
 	vector<int> y_vec = findY(bb_img); 
 	vector<int> y_c = findCutoffs(y_vec);

	//Extract letters
	extractLetters(bb_img, y_c, file_name);
 
	cvReleaseImage( &img ); 
 	cvReleaseImage( &out );
  	return EXIT_SUCCESS; 
} 
