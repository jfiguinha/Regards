#include "header.h"
#ifndef __APPLE__
#include <FreeImage.h>
#include <ximage.h>
#include <ConvertUtility.h>
#include <exiv2/image.hpp>
#include <exiv2/error.hpp>
#include "PictureMetadataExiv.h"
#include <libexif/exif-data.h>
using namespace Regards::exiv2;


CPictureMetadataExiv::CPictureMetadataExiv(const wxString& filename)
{
	//Copy des infos exifs
	isExif = false;

	//Read exif info from source file
	try
	{
		this->filename = filename;
		exif = Exiv2::ImageFactory::open(CConvertUtility::ConvertToStdString(filename));
		assert(exif.get() != 0);
		exif->readMetadata();
		isExif = true;
	}
	catch (Exiv2::AnyError& e)
	{
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
		//return -1;
	}
}

CPictureMetadataExiv::CPictureMetadataExiv(uint8_t* data, const long& size)
{
	//Copy des infos exifs
	isExif = false;

	//Read exif info from source file
	try
	{
		exif = Exiv2::ImageFactory::open(data, size);
		assert(exif.get() != 0);
		exif->readMetadata();
		isExif = true;
	}
	catch (Exiv2::AnyError& e)
	{
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
		//return -1;
	}
}


wxString CPictureMetadataExiv::GetCreationDate()
{
	wxString date = "";
	if (isExif)
	{
		try
		{
			Exiv2::ExifData& exifData = exif->exifData();
			if (!exifData.empty())
				return "";

			Exiv2::ExifKey orientationKey("Exif.Image.DateTime");
			Exiv2::ExifData::const_iterator md = exifData.findKey(orientationKey);
			if (exifData.end() != md)
			{
				date = md->value().toString();
			}
		}
		catch (...)
		{
		}
	}


	return date;
}

void CPictureMetadataExiv::SetOrientation(const int& orientation)
{
	if (exif.get() != nullptr)
	{
		Exiv2::ExifData& exifData = exif->exifData();
		if (!exifData.empty())
		{
			AddAsciiValue("Exif.Image.Orientation", to_string(orientation), exifData);
			exif->setExifData(exifData);
			exif->writeMetadata();
		}
		else
		{
			Exiv2::ExifData exif_data;
			AddAsciiValue("Exif.Image.Orientation", to_string(orientation), exif_data);
			exif->setExifData(exif_data);
			exif->writeMetadata();
		}
	}
}


void CPictureMetadataExiv::SetDateTime(const wxString& dateTime)
{
	Exiv2::ExifData& exifData = exif->exifData();
	if (!exifData.empty())
	{
		AddAsciiValue("Exif.Image.DateTime", dateTime, exifData);
		exif->setExifData(exifData);
		exif->writeMetadata();
	}
	else
	{
		Exiv2::ExifData exif_data;
		AddAsciiValue("Exif.Image.DateTime", dateTime, exif_data);
		exif->setExifData(exif_data);
		exif->writeMetadata();
	}
}

void CPictureMetadataExiv::AddAsciiValue(wxString keyName, wxString value, Exiv2::ExifData& exifData)
{
	Exiv2::ExifKey key(CConvertUtility::ConvertToStdString(keyName));
	if (!exifData.empty())
	{
		auto md = exifData.findKey(key);
		if (exifData.end() != md)
		{
			Exiv2::Value::AutoPtr rv = Exiv2::Value::create(Exiv2::asciiString);
			rv->read(CConvertUtility::ConvertToStdString(value));
			md->setValue(rv.get());
			return;
		}
	}

	// Create a ASCII string value (note the use of create)
	Exiv2::Value::AutoPtr v = Exiv2::Value::create(Exiv2::asciiString);
	// Set the value to a string
	v->read(CConvertUtility::ConvertToStdString(value));
	exifData.add(key, v.get());
}

void CPictureMetadataExiv::AddRationalValue(wxString keyName, wxString value, Exiv2::ExifData& exifData)
{
	Exiv2::ExifKey key(CConvertUtility::ConvertToStdString(keyName));
	if (!exifData.empty())
	{
		auto md = exifData.findKey(key);
		if (exifData.end() != md)
		{
			Exiv2::URationalValue::AutoPtr rv = GetGpsRationalValue(value);
			md->setValue(rv.get());
			return;
		}
	}
	// Create a ASCII string value (note the use of create)
	Exiv2::URationalValue::AutoPtr rv = GetGpsRationalValue(value);
	exifData.add(key, rv.get());
}

void CPictureMetadataExiv::SetGpsInfos(const wxString& latitudeRef, const wxString& longitudeRef,
                                       const wxString& latitude, const wxString& longitude)
{
	//Exiv2::ExifKey gpsTag("Exif.Image.GPSTag");
	//Exiv2::ExifKey gpsLatitudeRef("Exif.GPSInfo.GPSLatitudeRef"); //Ascii
	//Exiv2::ExifKey gpsLatitude("Exif.GPSInfo.GPSLatitude"); //Rational
	//Exiv2::ExifKey gpsLongitudeRef("Exif.GPSInfo.GPSLongitudeRef");//Ascii
	//Exiv2::ExifKey gpsLongitude("Exif.GPSInfo.GPSLongitude");//Rational


	Exiv2::ExifData& exifData = exif->exifData();
	if (!exifData.empty())
	{
		AddAsciiValue("Exif.GPSInfo.GPSLatitudeRef", latitudeRef, exifData);
		AddRationalValue("Exif.GPSInfo.GPSLatitude", latitude, exifData);
		AddAsciiValue("Exif.GPSInfo.GPSLongitudeRef", longitudeRef, exifData);
		AddRationalValue("Exif.GPSInfo.GPSLongitude", longitude, exifData);
		exif->setExifData(exifData);
		exif->writeMetadata();
	}
	else
	{
		Exiv2::ExifData exif_data;
		AddAsciiValue("Exif.GPSInfo.GPSLatitudeRef", latitudeRef, exif_data);
		AddRationalValue("Exif.GPSInfo.GPSLatitude", latitude, exif_data);
		AddAsciiValue("Exif.GPSInfo.GPSLongitudeRef", longitudeRef, exif_data);
		AddRationalValue("Exif.GPSInfo.GPSLongitude", longitude, exif_data);
		exif->setExifData(exif_data);
		exif->writeMetadata();
	}
}

CPictureMetadataExiv::~CPictureMetadataExiv()
{
}

bool CPictureMetadataExiv::HasExif()
{
	return isExif;
}


void CPictureMetadataExiv::GetMetadataBuffer(uint8_t*& data, unsigned int& size)
{
	/* raw EXIF header data */
	static const unsigned char exif_header[] = {
		0xff, 0xd8, 0xff, 0xe1
	};

	if (size == 0)
	{
		unsigned int local = 0;
		unsigned char* buf;
		ExifData* d = exif_data_new_from_file(filename);
		if (!d)
		{
			//fprintf(stderr, "Could not load data from '%s'!\n", filename);
			size = 0;
			return;
		}

		exif_data_save_data(d, &buf, &local);
		exif_data_unref(d);
		free(buf);
		size = local; // +sizeof(exif_header);
	}
	else
	{
		unsigned int local = 0;
		unsigned char* buf;
		ExifData* d = exif_data_new_from_file(filename);
		if (!d)
		{
			//fprintf(stderr, "Could not load data from '%s'!\n", filename);
			size = 0;
			return;
		}
		int pos = 0;
		unsigned char lenbuf[2];
		exif_data_save_data(d, &buf, &local);
		exif_data_unref(d);
		//memcpy(data, exif_header, sizeof(exif_header));
		//pos += sizeof(exif_header);
		memcpy(data, buf, local);

		free(buf);
	}
}

bool CPictureMetadataExiv::CopyMetadata(const wxString& output)
{
	if (isExif)
	{
		try
		{
			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(CConvertUtility::ConvertToStdString(output));
			if (exif.get())
			{
				Exiv2::ExifData& exifData = exif->exifData();
				if (!exifData.empty())
				{
					image->setExifData(exifData);
					image->writeMetadata();
				}
				else
				{
					Exiv2::XmpData& xmpData = image->xmpData();
					if (!xmpData.empty())
					{
						image->setXmpData(xmpData);
						image->writeMetadata();
					}
					else
					{
						Exiv2::IptcData& ipctData = image->iptcData();
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

bool CPictureMetadataExiv::HasThumbnail()
{
	if (exif.get() != nullptr)
	{
		Exiv2::ExifData& exifData = exif->exifData();
		if (!exifData.empty())
		{
			Exiv2::ExifThumb thumb(exifData);
			Exiv2::DataBuf data = thumb.copy();
			if (data.size_ > 0 && data.pData_ != nullptr)
				return true;
		}
	}


	return false;
}

int CPictureMetadataExiv::GetOrientation()
{
	int orientation = -1;
	if (isExif)
	{
		try
		{
			Exiv2::ExifData& exifData = exif->exifData();
			if (exifData.empty())
				return -1;

			Exiv2::ExifKey orientationKey("Exif.Image.Orientation");
			Exiv2::ExifData::const_iterator md = exifData.findKey(orientationKey);
			if (exifData.end() != md)
			{
				wxString value = md->value().toString();
				orientation = atoi(value.c_str());
			}
		}
		catch (...)
		{
		}
	}


	return orientation;
}

Exiv2::URationalValue::AutoPtr CPictureMetadataExiv::GetGpsRationalValue(const wxString& gpsValue)
{
	double dblValue;
	gpsValue.ToDouble(&dblValue);
	Exiv2::URationalValue::AutoPtr rv(new Exiv2::URationalValue);

	//Get Hour
	int result = lround(dblValue);
	rv->value_.push_back(std::make_pair(result, 1));

	//Get Minute
	dblValue = dblValue - result;
	dblValue = 60.0 * dblValue;
	result = lround(dblValue);
	rv->value_.push_back(std::make_pair(result, 1));

	//Get Seconds
	dblValue = dblValue - result;
	dblValue = (3600.0 * dblValue) * 100.0;
	result = lround(dblValue);
	rv->value_.push_back(std::make_pair(result, 100));
	return rv;
}

wxString CPictureMetadataExiv::GetGpsfValue(const wxString& gpsValue)
{
	wxString returnValue = "";
	vector<wxString> latValue;
	int i = 0;

	//Conversion des valeurs des latitudes et des longitudes
	latValue = CConvertUtility::split(gpsValue, ' ');

	float outputValue = 0.0;

	if (latValue.size() == 3)
	{
		for (auto it = latValue.begin(); it != latValue.end(); ++it)
		{
			vector<wxString> intValue = CConvertUtility::split(*it, '/');
			int valeur = atoi(intValue.at(0));
			int diviseur = atoi(intValue.at(1));

			float value = static_cast<float>(valeur) / static_cast<float>(diviseur);
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
	}

	return to_string(outputValue);
}


void CPictureMetadataExiv::ReadVideo(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
                                     wxString& longitude)
{
	hasGps = false;
	hasDataTime = false;
	if (isExif)
	{
		try
		{
			float flongitude;
			float flatitude;
			Exiv2::XmpData& xmpData = exif->xmpData();
			if (xmpData.empty())
				return;

			wxString altitude;
			hasGps = false;
			bool apple = false;
			wxString exifinfos;
			wxString informations;

			auto end = xmpData.end();
			for (auto md = xmpData.begin(); md != end; ++md)
			{
				informations = md->key();
				//long count = md->count();
				exifinfos = toString(*md);

				if (informations == "Xmp.video.MimeType")
				{
					if (exifinfos == "video/quicktime")
						apple = true;
				}
				else if (informations == "Xmp.video.GPSCoordinates")
				{
					hasGps = true;
					exifinfos = toString(*md);
					wxString listValue[3];
					wchar_t listRef[3];
					int iStart = -1;

					for (auto i = 0; i < exifinfos.size(); i++)
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
						flatitude = -atof(listValue[0]);
					else
						flatitude = atof(listValue[0]);

					if (listRef[1] == '-')
						flongitude = -atof(listValue[1]);
					else
						flongitude = atof(listValue[1]);

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
							dateTimeInfos = GetQuickTimeDate(dateTime);
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


wxString CPictureMetadataExiv::GetQuickTimeDate(int64_t dateQuicktime)
{
	char message[1024];
	//long timeFrom1970 = dateQuicktime - 2082844800;
	static const unsigned long SecsUntil1970 = 2082844800;

	struct tm MacTime = {0};
	unsigned long MacTimestamp;

	MacTimestamp = 3458306455; /* get timestamp: secs since 00:00 1904-01-01 GMT */

	/* Create unnormalized struct tm representing Macintosh timestamp */
	MacTime.tm_sec = static_cast<int>(MacTimestamp - SecsUntil1970);
	MacTime.tm_hour = 0; /* adjust this for your timezone */
	MacTime.tm_year = 70;

	/* Convert to time_t */
	mktime(&MacTime);
	strftime(message, 20, "%Y-%m-%dT%H:%M:%S", &MacTime);

	return message;
}


void CPictureMetadataExiv::ReadPicture(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
                                       wxString& longitude)
{
	hasGps = false;
	hasDataTime = false;
	if (isExif)
	{
		try
		{
			Exiv2::ExifData& exifData = exif->exifData();
			if (exifData.empty())
				return;

			wxString exifinfos;
			wxString informations;
			hasGps = false;
			latitude = "";
			wxString latitudeRef = "";
			longitude = "";
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
				dateTimeInfos = toString(*md);
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

				if (latitude != "" && longitude != "" && latitudeRef != "" && longitudeRef != "")
				{
					latitude = GetGpsfValue(latitude);
					longitude = GetGpsfValue(longitude);

					if (latitudeRef == "S")
						latitude = "-" + latitude;

					if (longitudeRef == "W")
						longitude = "-" + longitude;
				}
				else
					hasGps = false;
			}
			else
				hasGps = false;
		}
		catch (...)
		{
		}
	}
}

vector<CMetadata> CPictureMetadataExiv::ReadExif(Exiv2::ExifData& exifData)
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
		metadata.value = toString(*i);

		/*
		vector<wxString> listKey = CConvertUtility::split(metadata.key, '.');

		if (listKey.size() == 3)
		{
			if (listKey[2] == "MakerNote")
			{
				int len = metadata.value.length();
				wxString newString;
				for (int i = 0; i < len; i += 3)
				{
					wxString byte = metadata.value.substr(i, 2);
					char chr = (char)(int)strtol(byte.c_str(), nullptr, 16);
					newString.append(chr);
				}
				metadata.value = newString;
			}
		}
		*/
		metadataList.push_back(metadata);
	}
	return metadataList;
}

vector<CMetadata> CPictureMetadataExiv::ReadXmp(Exiv2::XmpData& xmpData)
{
	vector<CMetadata> metadataList;
	wxString altitude;
	wxString exifinfos;
	wxString informations;
	bool apple = false;
	auto end = xmpData.end();
	for (auto md = xmpData.begin(); md != end; ++md)
	{
		informations = md->key();
		//long count = md->count();
		exifinfos = toString(*md);

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

vector<CMetadata> CPictureMetadataExiv::ReadIpct(Exiv2::IptcData& ipctData)
{
	vector<CMetadata> metadataList;
	//int item = 0;
	//int index = 0;
	wxString exifinfos;
	wxString informations;
	auto end = ipctData.end();
	for (auto md = ipctData.begin(); md != end; ++md)
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
				auto out = new wchar_t[size + 1];
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

vector<CMetadata> CPictureMetadataExiv::GetMetadata()
{
	vector<CMetadata> metadataList;
	if (isExif && exif.get() != nullptr)
	{
		try
		{
			Exiv2::ExifData& exifData = exif->exifData();
			Exiv2::IptcData& ipctData = exif->iptcData();
			Exiv2::XmpData& xmpData = exif->xmpData();
			if (!exifData.empty())
			{
				metadataList = ReadExif(exifData);
			}
			else if (!ipctData.empty())
			{
				metadataList = ReadIpct(ipctData);
			}
			else if (!xmpData.empty())
			{
				metadataList = ReadXmp(xmpData);
			}
		}
		catch (...)
		{
		}
	}
	return metadataList;
}


wxImage CPictureMetadataExiv::LoadThumbnailFromExif(Exiv2::ExifData* dataIn, wxString& extension,
                                                                 int& orientation)
{
	wxImage image;
	if (dataIn != nullptr)
	{
		Exiv2::ExifThumb thumb(*dataIn);
		extension = thumb.extension();
		extension = extension.substr(1, extension.size() - 1);
		Exiv2::DataBuf data = thumb.copy();
		if (data.size_ > 0 && data.pData_ != nullptr)
		{
			Exiv2::ExifKey orientationKey("Exif.Image.Orientation");
			Exiv2::ExifData::const_iterator md = dataIn->findKey(orientationKey);
			if (dataIn->end() != md)
			{
				wxString value = md->value().toString();
				orientation = atoi(value.c_str());
			}

			wxMemoryInputStream cxMemFile(data.pData_, data.size_);
			image.LoadFile(cxMemFile, wxBITMAP_TYPE_ANY);
		}
	}
	return image;
}

wxImage CPictureMetadataExiv::DecodeThumbnail(wxString& extension, int& orientation)
{
	wxImage bitmap;
	try
	{
		Exiv2::ExifData& exifData = exif->exifData();

		if (!exifData.empty())
		{
			bitmap = LoadThumbnailFromExif(&exifData, extension, orientation);
		}
	}
	catch (...)
	{
	}
	return bitmap;
}
#endif