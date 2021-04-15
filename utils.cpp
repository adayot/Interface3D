#include "utils.h"
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;
string getTagType(const dcm::DataElement* element)
{
    switch (element->vr().code())
    {
      case 16709: // AE
      case 16723: // AS
      case 17235: // CS
      case 17473: // DA
      case 17491: // DS
      case 17492: // DT
      case 18771: // IS
      case 19535: // LO
      case 19540: // LT
      case 20290: // OB
      case 20292: // OD
      case 20294: // OF
      case 20311: // OW
      case 20558: // PN
      case 21320: // SH
      case 21332: // ST
      case 21581: // TM
      case 21833: // UI
        return "string";
      case 21843: // US
        return "unsigned short";
      case 21331: // SS
        return "short";
      case 21836: // UL
        return "unsigned int";
      case 21324: // SL
        return "int";
      case 17996: // FL
        return "float";
      case 17484: // FD
        return "double";
      default: return "unknown";
    }
}

string getStringTagValue(unsigned int address, dcm::DicomFile* data)
{
  string value;

  data->GetString(address, &value);
  return value;
}

unsigned short getUShortTagValue(unsigned int address, dcm::DicomFile* data)
{
  unsigned short value;

  data->GetUint16(address, &value);
  return value;
}

short getShortTagValue(unsigned int address, dcm::DicomFile* data)
{
  short value;

  data->GetInt16(address, &value);
  return value;
}

unsigned int getUIntTagValue(unsigned int address, dcm::DicomFile* data)
{
  unsigned int value;

  data->GetUint32(address, &value);
  return value;
}

int getIntTagValue(unsigned int address, dcm::DicomFile* data)
{
  int value;

  data->GetInt32(address, &value);
  return value;
}

float getFloatTagValue(unsigned int address, dcm::DicomFile* data)
{
  float value;

  data->GetFloat32(address, &value);
  return value;
}

double getDoubleTagValue(unsigned int address, dcm::DicomFile* data)
{
  double value;

  data->GetFloat64(address, &value);
  return value;
}

vector<unsigned short>* readPixels(dcm::DicomFile* data)
{
  vector<unsigned short>* pixels;
  const dcm::DataElement* pixelsElement;
  
  pixelsElement = data->Get(0x7fe00010);
  pixels = new vector<unsigned short>(pixelsElement->length() / 2);
  for (auto i = 0; i < pixelsElement->length(); i+=2)
  {
    unsigned char part1 = pixelsElement->buffer()[i];
    unsigned char part2 = pixelsElement->buffer()[i + 1];
    if (part2 == 255)
        (*pixels)[i / 5] = (*pixels)[i / 5];
    else
      (*pixels)[i/2] = (part2 << 8) | part1;
  }
  return pixels;
}

dcm::DicomFile* readFile(const char* filePath)
{
  dcm::DicomFile* dicom_file = new dcm::DicomFile(filePath);
  if (!dicom_file->Load())
  {
    cout << "Failed to read file." << endl;
    return NULL;
  }
  return dicom_file;
}


void displayPixels(vector<unsigned short> *pixels)
{
  for (auto pixel: *pixels)
    cout << pixel << " ";
  cout << endl;
}

