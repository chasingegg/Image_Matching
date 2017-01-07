#include "Common/Common.h"
#include "Common/Log.h"
#include "Common/Image.h"
#include "Features/FeatureHandler.h"

#include <dirent.h>
#include <set>

#include <opencv2/calib3d/calib3d.hpp>
#include <fstream>
#include <iostream>

Image* processImage( std::string path );
void findObjectInScene( std::string path );

void testDir( std::string path1, std::string path2, std::ofstream &outFile );
void getFileList(std::string path);
void getFileListIn(std::string path);

FeatureHandler* _fH;
float _ratio;
Image* _object;
std::string _path;

extern int final_size;

std::set< std::string > _infiles;
std::set< std::string > _files;

bool Log::debug = true;

int main( int argc, char** argv )
{
	int status = EXIT_SUCCESS;
	std::ofstream outFile;
	outFile.open("result");

	if(argc >= 3)
	{
		getFileListIn(argv[1]);
		std::set< std::string >::iterator it = _infiles.begin();
		for( ; it != _infiles.end(); it++ )
		{
			std::string aa = "/";
			std::string file_path = argv[1]+aa+*it;
			_fH = new FeatureHandler( DET_SIFT, DES_SIFT, DESM_FB );
			_object = processImage(file_path);
			_ratio = 0.85f;
			testDir(*it, argv[2],outFile);
		}		
	}
	else
	{
		status = EXIT_FAILURE;
		std::cout << "Problem lauching program." << std::endl
				<< "USAGE: objectDetector <path object image> <path to test dir>" << std::endl;
	}

	return status;
}

void findObjectInScene( std::string path )
{
	Image* scene = processImage(path);

	_fH->findGoodMatches( _object, scene, _ratio );
	std::vector< cv::DMatch > goodMatches = _object->getGoodMatches( scene );
	int nMatches = goodMatches.size();

	cv::Mat out;

	Log::notice("Good matches: "+Log::to_string(int(final_size)));

	if (final_size>4)
		_object->drawMatches( scene, nMatches, MA_GD, out );
	else
		{
			std::cout<<"Too few matches.\n";
			return ;
		}

	//-- Localize the object
	std::vector<cv::Point2f> inObj;
	std::vector<cv::Point2f> inScene;

	for( int i = 0; i < nMatches; i++ )
	{
		//-- Get the keypoints from the good matches
		inObj.push_back( _object->getKeypoints()[ goodMatches[i].queryIdx ].pt );
		inScene.push_back( scene->getKeypoints()[ goodMatches[i].trainIdx ].pt );
	}


	//cv::namedWindow("Matches", CV_WINDOW_NORMAL); 
	//cv::imshow("Matches", out); 
	//cv::waitKey(0);
}

Image* processImage( std::string path )
{
	Image* img = new Image( path, 0.0f, 0.0f, 0.0f, 0.0f );

	_fH->detect( img );
	_fH->computeDescriptors( img );

	return img;
}

void testDir(std::string path1, std::string path2, std::ofstream &outFile )
{
	getFileList(path2);

	std::set< std::string >::iterator it = _files.begin();
	for( ; it != _files.end(); it++ )
	{
		findObjectInScene( path2+"/"+*it );
		if (final_size > 50)
		{
			//outFile << path1 << '\t' <<*it << '\t'<< final_size<<std::endl;
			outFile<<1<<std::endl;
		}
		else
		{
				outFile<<0<<std::endl;
		}
	}
}

void getFileList(std::string path)
{
	DIR *dir;
	struct dirent *ent;
	if( ( dir = opendir( path.c_str() ) ) != NULL )
	{
		while( ( ent = readdir( dir ) ) != NULL)
		{
			if( ent->d_type == DT_REG )
			{
				_files.insert( std::string( ent->d_name ) );
			}
		}
		closedir (dir);
	}
	else
	{
	  Log::exit_error("Unable to access directory " + _path );
	}
}

void getFileListIn(std::string path)
{
	DIR *dir;
	struct dirent *ent;
	if( ( dir = opendir( path.c_str() ) ) != NULL )
	{
		while( ( ent = readdir( dir ) ) != NULL)
		{
			if( ent->d_type == DT_REG )
			{
				_infiles.insert( std::string( ent->d_name ) );
			}
		}
		closedir (dir);
	}
	else
	{
	  Log::exit_error("Unable to access directory " + _path );
	}
}