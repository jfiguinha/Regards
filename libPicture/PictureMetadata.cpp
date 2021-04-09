#include <header.h>
#include "PictureMetadata.h"
#include <LibResource.h>
#include <ConvertUtility.h>
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <exiv2/iptc.hpp>
#include <xmemfile.h>
#include <ximage.h>
#include <wx/mstream.h>
using namespace Exiv2;

class CExiv
{
public:
	CxImage * LoadThumbnailFromExif(Exiv2::ExifData * dataIn, wxString &extension);
	wxString GetQuickTimeDate(int64_t dateQuicktime);
	vector<CMetadata> ReadExif(Exiv2::ExifData &exifData);
	vector<CMetadata> ReadIpct(Exiv2::IptcData &ipctData);
	vector<CMetadata> ReadXmp(Exiv2::XmpData &xmpData);
	Exiv2::Image::AutoPtr exif;
	wxString GetGpsfValue(const wxString& gpsValue);
};

wxString CExiv::GetGpsfValue(const wxString& gpsValue)
{
	wxString returnValue = "";
	vector<wxString> latValue;
	int i = 0;

	//Conversion des valeurs des latitudes et des longitudes
	latValue = CConvertUtility::split(gpsValue, ' ');

	float outputValue = 0.0;

	for (vector<wxString>::iterator it = latValue.begin(); it != latValue.end(); it++)
	{

		vector<wxString> intValue = CConvertUtility::split(*it, '/');
		int valeur = atoi(intValue.at(0).c_str());
		int diviseur = atoi(intValue.at(1).c_str());

		float value = (float)valeur / (float)diviseur;
		if (i == 1)
		{
			value = value / 60;
		}
		else if (i == 2)
		{
			value = value / 3600;
		}

		outputValue += value;
		i++;
	}

	return  std::to_string(outputValue);
}

CPictureMetadata::CPictureMetadata(const wxString &filename)
{
	//Copy des infos exifs
	exivData = new CExiv();
	
	//Read exif info from source file
	try
	{
		exivData->exif = Exiv2::ImageFactory::open(filename.ToStdString());
		exivData->exif->readMetadata();
		isExif = true;

	}
	catch (...)
	{
		isExif = false;
	}

}

CPictureMetadata::~CPictureMetadata()
{
	delete exivData;
}

bool CPictureMetadata::HasExif()
{
	return isExif;
}

bool CPictureMetadata::CopyMetadata(const wxString &output)
{
	if (isExif)
	{
		try
		{
			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(output.ToStdString());
			if (exivData->exif.get())
			{
				Exiv2::ExifData &exifData = exivData->exif->exifData();
				if (!exifData.empty())
				{
					image->setExifData(exifData);
					image->writeMetadata();
				}
				else
				{
					Exiv2::XmpData &xmpData = image->xmpData();
					if (!xmpData.empty())
					{
						image->setXmpData(xmpData);
						image->writeMetadata();
					}
					else
					{
						Exiv2::IptcData &ipctData = image->iptcData();
						if (!ipctData.empty())
						{
							image->setIptcData(ipctData);
							image->writeMetadata();
						}
					}
				}
				return true;
			}
		}
		catch (...)
		{

		}
	}
	return false;
}

bool CPictureMetadata::HasThumbnail()
{
	if (isExif)
	{
		Exiv2::ExifData &exifData = exivData->exif->exifData();
		if (!exifData.empty())
			return true;
	}
	return false;
}


void CPictureMetadata::ReadVideo(bool & hasGps, bool & hasDataTime, wxString &dateTimeInfos, wxString &latitude, wxString &longitude)
{
	float flatitude = 0.0;
	float flongitude = 0.0;
	hasGps = false;
	hasDataTime = false;
	if (isExif)
	{
		try
		{
			Exiv2::XmpData &xmpData = exivData->exif->xmpData();
			if (xmpData.empty())
				return;

			wxString altitude;
			hasGps = false;
			bool apple = false;
			wxString exifinfos;
			wxString informations;

			Exiv2::XmpData::iterator end = xmpData.end();
			for (Exiv2::XmpData::iterator md = xmpData.begin(); md != end; ++md)
			{
				informations = md->key();
				//long count = md->count();
				exifinfos = Exiv2::toString(*md);

				if (informations == "Xmp.video.MimeType")
				{
					if (exifinfos == "video/quicktime")
						apple = true;
				}
				else if (informations == "Xmp.video.GPSCoordinates")
				{
					hasGps = true;
					exifinfos = Exiv2::toString(*md);
					wxString listValue[3];
					wchar_t listRef[3];
					int iStart = -1;

					for (int i = 0; i < exifinfos.size(); i++)
					{
						char charValue = exifinfos.at(i);
						if (charValue == '+')
						{
							iStart++;
							listRef[iStart] = '+';
						}
						else if (charValue == '-')
						{
							iStart++;
							listRef[iStart] = '-';
						}
						else
						{
							listValue[iStart] += charValue;
						}
					}

					if (listRef[0] == '-')
						flatitude = -atof(listValue[0].c_str());
					else
						flatitude = atof(listValue[0].c_str());

					if (listRef[1] == '-')
						flongitude = -atof(listValue[1].c_str());
					else
						flongitude = atof(listValue[1].c_str());

					latitude = to_string(flatitude);
					longitude = to_string(flongitude);


				}
				else if (informations.Find("TrackCreateDate") >= 0)
				{
					if (apple)
					{
						int64_t dateTime = atol(exifinfos.c_str());
						if (dateTime > 0)
						{
							dateTimeInfos = exivData->GetQuickTimeDate(dateTime);
							hasDataTime = true;
						}
					}
					else
					{
						dateTimeInfos = exifinfos;
						hasDataTime = true;
					}

				}
			}
		}
		catch (...)
		{
		}
	}

}


wxString CExiv::GetQuickTimeDate(int64_t dateQuicktime)
{
	char message[1024];
	//long timeFrom1970 = dateQuicktime - 2082844800;
	static const unsigned long SecsUntil1970 = 2082844800;

	struct tm MacTime = { 0 };
	unsigned long MacTimestamp;
	time_t CTime;

	MacTimestamp = 3458306455; /* get timestamp: secs since 00:00 1904-01-01 GMT */
	//if (MacTimestamp < SecsUntil1970)
	/* handle separately... */;

	/* Create unnormalized struct tm representing Macintosh timestamp */
	MacTime.tm_sec = (int)(MacTimestamp - SecsUntil1970);
	MacTime.tm_hour = 0; /* adjust this for your timezone */
	MacTime.tm_year = 70;

	/* Convert to time_t */
	CTime = mktime(&MacTime);
	strftime(message, 20, "%Y-%m-%dT%H:%M:%S", &MacTime);

	return message;
}


void CPictureMetadata::ReadPicture(bool & hasGps, bool & hasDataTime, wxString &dateTimeInfos, wxString &latitude, wxString &longitude)
{
	hasGps = false;
	hasDataTime = false;
	if (isExif)
	{
		try
		{
			Exiv2::ExifData &exifData = exivData->exif->exifData();
			if (exifData.empty())
				return;

			wxString exifinfos;
			wxString informations;
			hasGps = false;
			wxString latitudeRef = "";
			wxString longitudeRef = "";

			Exiv2::ExifKey gpsTag("Exif.Image.GPSTag");
			Exiv2::ExifKey gpsLatitudeRef("Exif.GPSInfo.GPSLatitudeRef");
			Exiv2::ExifKey gpsLatitude("Exif.GPSInfo.GPSLatitude");
			Exiv2::ExifKey gpsLongitudeRef("Exif.GPSInfo.GPSLongitudeRef");
			Exiv2::ExifKey gpsLongitude("Exif.GPSInfo.GPSLongitude");
			Exiv2::ExifKey dateTime("Exif.Image.DateTime");

			Exiv2::ExifData::const_iterator md = exifData.findKey(dateTime);
			if (exifData.end() != md)
			{
				hasDataTime = true;
				dateTimeInfos = Exiv2::toString(*md);
			}

			md = exifData.findKey(gpsTag);
			if (exifData.end() != md)
			{
				hasGps = true;
				md = exifData.findKey(gpsLatitudeRef);
				if (exifData.end() != md)
				{
					latitudeRef = md->value().toString();
				}
				md = exifData.findKey(gpsLatitude);
				if (exifData.end() != md)
				{
					latitude = md->value().toString();
				}
				md = exifData.findKey(gpsLongitudeRef);
				if (exifData.end() != md)
				{
					longitudeRef = md->value().toString();
				}
				md = exifData.findKey(gpsLongitude);
				if (exifData.end() != md)
				{
					longitude = md->value().toString();
				}
				
				latitude = exivData->GetGpsfValue(latitude);
				longitude = exivData->GetGpsfValue(longitude);
                
                if (latitudeRef == "S")
                    latitude = "-" + latitude;
    
                if (longitudeRef == "W")
                    longitude = "-" + longitude;

				//printf("longitude %s \n",longitude.ToStdString().c_str());
				//printf("latitude %s \n",latitude.ToStdString().c_str());
			}
			else
				hasGps = false;
		}
		catch (...)
		{
		}
	}
}

vector<CMetadata> CExiv::ReadExif(Exiv2::ExifData &exifData)
{
	vector<CMetadata> metadataList;
	//int item = 0;
	//int index = 0;
	wxString exifinfos;
	wxString informations;

	Exiv2::ExifData::const_iterator end = exifData.end();

	for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
	{
		CMetadata metadata;
		metadata.key = i->key();
		metadata.value = Exiv2::toString(*i);
		metadataList.push_back(metadata);

	}
	return metadataList;
}

vector<CMetadata> CExiv::ReadXmp(Exiv2::XmpData &xmpData)
{
	vector<CMetadata> metadataList;
	wxString altitude;
	wxString exifinfos;
	wxString informations;
	bool apple = false;
	Exiv2::XmpData::iterator end = xmpData.end();
	for (Exiv2::XmpData::iterator md = xmpData.begin(); md != end; ++md)
	{
		informations = md->key();
		//long count = md->count();
		exifinfos = Exiv2::toString(*md);

		if (informations == "Xmp.video.MimeType" && exifinfos == "video/quicktime")
		{
			apple = true;
		}
		if (informations.Find("Date") >= 0 && apple)
		{
			int64_t dateTime = atol(exifinfos.c_str());
			if (dateTime > 0)
				exifinfos = GetQuickTimeDate(dateTime);
		}
 

		CMetadata metadata;
		metadata.key = informations;
		metadata.value = exifinfos;
		metadataList.push_back(metadata);

	}
	return metadataList;
}

vector<CMetadata> CExiv::ReadIpct(Exiv2::IptcData &ipctData)
{
	vector<CMetadata> metadataList;
	//int item = 0;
	//int index = 0;
	wxString exifinfos;
	wxString informations;
	Exiv2::IptcData::iterator end = ipctData.end();
	for (Exiv2::IptcData::iterator md = ipctData.begin(); md != end; ++md)
	{
		informations = md->key();
		exifinfos = md->value().toString();

		if (md->typeId() == Exiv2::TypeId::unsignedByte)
		{
			Exiv2::byte data[1024];
			Exiv2::Value::AutoPtr value = md->getValue();
			if (value.get())
			{
				long size = value->copy(data, Exiv2::ByteOrder::invalidByteOrder);
				wchar_t * out = new wchar_t[size + 1];
				memcpy(out, data, size);
				out[size / sizeof(wchar_t)] = '\0';
				exifinfos = out;
				delete[] out;
			}
			else
			{
				exifinfos[0] = '\0';
			}

			CMetadata metadata;
			metadata.key = informations;
			metadata.value = exifinfos;
			metadataList.push_back(metadata);
		}
	}
	return metadataList;
}

vector<CMetadata> CPictureMetadata::GetMetadata()
{
	vector<CMetadata> metadataList;
	if (isExif)
	{
		Exiv2::ExifData &exifData = exivData->exif->exifData();
		Exiv2::IptcData &ipctData = exivData->exif->iptcData();
		Exiv2::XmpData &xmpData = exivData->exif->xmpData();
		if (!exifData.empty())
		{
			metadataList = exivData->ReadExif(exifData);
		}
		else if (!ipctData.empty())
		{
			metadataList = exivData->ReadIpct(ipctData);
		}
		else if (!xmpData.empty())
		{
			metadataList = exivData->ReadXmp(xmpData);
		}

	}
	return metadataList;
}


CxImage * CExiv::LoadThumbnailFromExif(Exiv2::ExifData * dataIn, wxString &extension)
{
	CxImage * image = nullptr;
	Exiv2::ExifThumb thumb(*dataIn);
	extension = thumb.extension();
	Exiv2::DataBuf data = thumb.copy();
	if (data.size_ > 0 && data.pData_ != nullptr)
	{
		CxMemFile * cxMemFile = new CxMemFile(data.pData_, data.size_);
		
		if(extension == ".jpg")
		{
			image = new CxImage(cxMemFile, CxImage::GetTypeIdFromName("jpg"));
		}
		else if(extension == ".tif")
		{
			image = new CxImage(cxMemFile, CxImage::GetTypeIdFromName("tif"));
		}
		else if(extension == ".bmp")
		{
			image = new CxImage(cxMemFile, CxImage::GetTypeIdFromName("bmp"));
		}
			
		wxString error = image->GetLastError();
		printf("Message : %s\n",error.ToStdString().c_str());
	}


	return image;
}

CxImage * CPictureMetadata::DecodeThumbnail(wxString &extension)
{
	CxImage * bitmap = nullptr;
	if (isExif)
	{
		try
		{
			Exiv2::ExifData &exifData = exivData->exif->exifData();

			if (!exifData.empty())
			{
				bitmap = exivData->LoadThumbnailFromExif(&exifData, extension);
			}
		}
		catch (...)
		{
		}
	}
	return bitmap;
}


int CPictureMetadata::Orientation()
{
	int orientation = 0;
	if (isExif)
	{
		try
		{
			Exiv2::ExifData &exifData = exivData->exif->exifData();

			if (!exifData.empty())
			{
				Exiv2::ExifKey orientationKey("Exif.Image.Orientation");
				Exiv2::ExifData::const_iterator md = exifData.findKey(orientationKey);
				if (exifData.end() != md)
				{
					wxString value = Exiv2::toString(*md);
					if(value == "top, left")
						orientation = 1;
					else if(value == "top, right")
						orientation = 2;
					else if(value == "bottom, right")
						orientation = 3;
					else if(value == "bottom, left")
						orientation = 4;
					else if(value == "left, top")
						orientation = 5;
					else if(value == "right, top")
						orientation = 6;
					else if(value == "right, bottom")
						orientation = 7;
					else if(value == "left, bottom")
						orientation = 8;					
					printf("orientation value : %s \n",value.ToStdString().c_str());
					//orientation = atoi(value.c_str());
				}
			}
		}
		catch (...)
		{
		}
	}	

    return orientation;
}