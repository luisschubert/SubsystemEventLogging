// writeToFile.cpp
#include <fstream>
#include <iostream>

class FileWriter{
	public:
	FileWriter();
	int doLog(std::string&);
	
	private:
	std::string filename;
};
FileWriter::FileWriter(){
	filename = "log.txt";
}
int FileWriter::doLog(std::string& input){
	std::ofstream outfile;
	outfile.open(filename, std::ios_base::app);
	outfile << input << std::endl;	
	// DOES NOT ACTUALLY CHECK IF THE CREATION WAS SUCCESSFUL JUST ASSUMES THAT IT IS.... NOT GOOD.
	return 1;
}

int main() {
	FileWriter Log;
	std::string testString = "TEST LOG OUTPUT";
	if(Log.doLog(testString))
		std::cout << "Successful writing" << std::endl;
	else
		std::cout << "Unsuccessful writing" << std::endl;
	return 0;

}
