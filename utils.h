// C++ includes.
#include <iostream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>

// DCM includes.
#include <dcm/data_dict.h>
#include <dcm/dicom_file.h>

// Namespaces.
using namespace std;

// Functions.
string getTagType(const dcm::DataElement* element);
string getStringTagValue(unsigned int address, dcm::DicomFile* data);
unsigned short getUShortTagValue(unsigned int address, dcm::DicomFile* data);
short getShortTagValue(unsigned int address, dcm::DicomFile* data);
unsigned int getUIntTagValue(unsigned int address, dcm::DicomFile* data);
int getIntTagValue(unsigned int address, dcm::DicomFile* data);
float getFloatTagValue(unsigned int address, dcm::DicomFile* data);
double getDoubleTagValue(unsigned int address, dcm::DicomFile* data);
vector<unsigned short>* readPixels(dcm::DicomFile* data);
dcm::DicomFile* readFile(const char* filePath);
void displayPixels(vector<unsigned short>* pixels);
