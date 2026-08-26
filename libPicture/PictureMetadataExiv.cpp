#include "header.h"
#include <ConvertUtility.h>
#include <exiv2/image.hpp>
#include <exiv2/error.hpp>
#include "PictureMetadataExiv.h"
#include <libexif/exif-data.h>
#include <regex>
using namespace Regards::exiv2;


CPictureMetadataExiv::CPictureMetadataExiv(const wxString& filename)
{
	//Copy des infos exifs
	isExif = false;

	static std::mutex exivMutex;
	try
	{
		std::lock_guard<std::mutex> lock(exivMutex);

		exif = Exiv2::ImageFactory::open(
			CConvertUtility::ConvertToStdString(filename));

		if (exif)
			exif->readMetadata();

		if (exif)
			isExif = true;
	}
	catch (std::exception e1)
	{

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
		//assert(exif.get() != 0);
		exif->readMetadata();
		isExif = true;
	}
	catch (Exiv2::Error& e)
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
			if (exifData.empty())
				return "";

			Exiv2::ExifKey orientationKey("Exif.Image.DateTime");
			Exiv2::ExifData::const_iterator md = exifData.findKey(orientationKey);
			if (exifData.end() != md)
			{
				date = md->value().toString();
			}
		}
		catch(const std::exception& e)
		{
		}
	}


	return date;
}

void CPictureMetadataExiv::SetOrientation(const int& orientation)
{
	if (!isExif || exif.get() == nullptr)
		return;

	try
	{
		Exiv2::ExifData& exifData = exif->exifData();
		AddAsciiValue("Exif.Image.Orientation", to_string(orientation), exifData);
		exif->setExifData(exifData);
		exif->writeMetadata();
	}
	catch(const std::exception& e)
	{
	}
}


void CPictureMetadataExiv::SetDateTime(const wxString& dateTime)
{
	if (!isExif || exif.get() == nullptr)
		return;

	try
	{
		Exiv2::ExifData& exifData = exif->exifData();
		AddAsciiValue("Exif.Image.DateTime", dateTime, exifData);
		exif->setExifData(exifData);
		exif->writeMetadata();
	}
	catch(const std::exception& e)
	{
	}
}

void CPictureMetadataExiv::AddAsciiValue(wxString keyName, wxString value, Exiv2::ExifData& exifData)
{
	Exiv2::ExifKey key(CConvertUtility::ConvertToStdString(keyName));

	auto md = exifData.findKey(key);
	if (exifData.end() != md)
	{
		Exiv2::Value::UniquePtr rv = Exiv2::Value::create(Exiv2::asciiString);
		rv->read(CConvertUtility::ConvertToStdString(value));
		md->setValue(rv.get());
		return;
	}

	// Create a ASCII string value (note the use of create)
	Exiv2::Value::UniquePtr v = Exiv2::Value::create(Exiv2::asciiString);
	// Set the value to a string
	v->read(CConvertUtility::ConvertToStdString(value));
	exifData.add(key, v.get());
}

void CPictureMetadataExiv::AddRationalValue(wxString keyName, wxString value, Exiv2::ExifData& exifData)
{
	Exiv2::ExifKey key(CConvertUtility::ConvertToStdString(keyName));

	auto md = exifData.findKey(key);
	if (exifData.end() != md)
	{
		Exiv2::URationalValue::UniquePtr rv = GetGpsRationalValue(value);
		md->setValue(rv.get());
		return;
	}

	Exiv2::URationalValue::UniquePtr rv = GetGpsRationalValue(value);
	exifData.add(key, rv.get());
}

void CPictureMetadataExiv::SetGpsInfos(const wxString& latitudeRef, const wxString& longitudeRef,
	const wxString& latitude, const wxString& longitude)
{
	if (!isExif || exif.get() == nullptr)
		return;

	try
	{
		Exiv2::ExifData& exifData = exif->exifData();

		AddAsciiValue("Exif.GPSInfo.GPSLatitudeRef", latitudeRef, exifData);
		AddRationalValue("Exif.GPSInfo.GPSLatitude", latitude, exifData);
		AddAsciiValue("Exif.GPSInfo.GPSLongitudeRef", longitudeRef, exifData);
		AddRationalValue("Exif.GPSInfo.GPSLongitude", longitude, exifData);

		exif->setExifData(exifData);
		exif->writeMetadata();
	}
	catch(const std::exception& e)
	{
	}
}

CPictureMetadataExiv::~CPictureMetadataExiv()
{}

bool CPictureMetadataExiv::HasExif()
{
	return isExif;
}


// Recupere les donnees Exif brutes du fichier (telles que produites par
// libexif), au format pret a etre ecrit comme segment APP1 (sans le
// header JPEG/Exif "ff d8 ff e1" + taille, qui n'est pas ajoute ici).
//
// Protocole en deux passes, a la charge de l'appelant :
//   1) appeler avec size == 0 : la fonction renvoie dans "size" la taille
//      necessaire (0 si aucune donnee Exif n'a pu etre lue).
//   2) l'appelant alloue un buffer "data" d'au moins "size" octets, puis
//      rappelle la fonction avec ce "size" : la fonction remplit "data".
//
// Pour eviter de relire/re-parser le fichier deux fois (et pour garantir
// que la taille annoncee a la passe 1 correspond exactement aux donnees
// ecrites a la passe 2, meme si le fichier change entre les deux appels),
// le buffer lu par libexif est mis en cache dans "cachedMetadataBuffer"
// des la premiere passe et reutilise par la seconde.
std::vector<uint8_t> CPictureMetadataExiv::GetMetadataBuffer()
{
	std::vector<uint8_t> buffer;

	ExifData* d = exif_data_new_from_file(filename);
	if (!d)
	{
		return buffer;
	}

	unsigned char* buf = nullptr;
	unsigned int local = 0;
	exif_data_save_data(d, &buf, &local);
	exif_data_unref(d);

	if (buf != nullptr && local > 0)
	{
		buffer.assign(buf, buf + local);
	}

	if (buf != nullptr)
		free(buf);

	return buffer;
}

bool CPictureMetadataExiv::CopyMetadata(const wxString& output)
{
	if (isExif)
	{
		try
		{
			Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(CConvertUtility::ConvertToStdString(output));
			if (exif.get())
			{
				bool wroteSomething = false;

				Exiv2::ExifData& exifData = exif->exifData();
				if (!exifData.empty())
				{
					image->setExifData(exifData);
					wroteSomething = true;
				}

				Exiv2::XmpData& xmpData = exif->xmpData();
				if (!xmpData.empty())
				{
					image->setXmpData(xmpData);
					wroteSomething = true;
				}

				Exiv2::IptcData& iptcData = exif->iptcData();
				if (!iptcData.empty())
				{
					image->setIptcData(iptcData);
					wroteSomething = true;
				}

				if (wroteSomething)
				{
					image->writeMetadata();
				}

				return true;
			}
		}
		catch(const std::exception& e)
		{
		}
	}
	return false;
}

bool CPictureMetadataExiv::HasThumbnail()
{
	if (isExif && exif.get() != nullptr)
	{
		try
		{
			Exiv2::ExifData& exifData = exif->exifData();
			if (!exifData.empty())
			{
				Exiv2::ExifThumb thumb(exifData);
				Exiv2::DataBuf data = thumb.copy();
				if (data.size() > 0 && data.data() != nullptr)
					return true;
			}
		}
		catch(const std::exception& e)
		{
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
		catch (const std::exception& e)
		{
		}
	}


	return orientation;
}

Exiv2::URationalValue::UniquePtr CPictureMetadataExiv::GetGpsRationalValue(const wxString& gpsValue)
{
	double decimalDegrees = 0.0;
	if (!gpsValue.ToDouble(&decimalDegrees))
		return std::make_unique<Exiv2::URationalValue>();

	decimalDegrees = std::abs(decimalDegrees);

	const uint32_t degrees = static_cast<uint32_t>(std::floor(decimalDegrees));

	const double minutesDecimal = (decimalDegrees - degrees) * 60.0;
	const uint32_t minutes = static_cast<uint32_t>(std::floor(minutesDecimal));

	const double secondsDecimal = (minutesDecimal - minutes) * 60.0;
	const uint32_t seconds = static_cast<uint32_t>(std::round(secondsDecimal * 10000.0));

	auto value = std::make_unique<Exiv2::URationalValue>();

	value->value_.emplace_back(degrees, 1);
	value->value_.emplace_back(minutes, 1);
	value->value_.emplace_back(seconds, 10000);

	return value;
}

wxString CPictureMetadataExiv::GetGpsfValue(const wxString& gpsValue)
{
	std::istringstream iss(CConvertUtility::ConvertToStdString(gpsValue));

	double decimalDegrees = 0.0;

	for (int i = 0; i < 3; ++i)
	{
		std::string token;
		if (!(iss >> token))
			return "";

		const auto pos = token.find('/');
		if (pos == std::string::npos)
			return "";

		const double numerator = std::stod(token.substr(0, pos));
		const double denominator = std::stod(token.substr(pos + 1));

		if (denominator == 0.0)
			return "";

		const double value = numerator / denominator;

		switch (i)
		{
		case 0:
			decimalDegrees += value;
			break;
		case 1:
			decimalDegrees += value / 60.0;
			break;
		case 2:
			decimalDegrees += value / 3600.0;
			break;
		}
	}

	return wxString::Format("%.8f", decimalDegrees);
}



Exiv2::ExifData* CPictureMetadataExiv::GetExifData()
{
	if (!exif)
		return nullptr;

	Exiv2::ExifData& exifData = exif->exifData();

	if (exifData.empty())
		return nullptr;

	return &exifData;
}

void CPictureMetadataExiv::ReadPicture(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
	wxString& longitude)
{
	hasGps = false;
	hasDataTime = false;

	auto* exifData = GetExifData();
	if (!exifData || exifData->empty())
		return;

	hasDataTime = ReadDateTime(*exifData, dateTimeInfos);
	hasGps = ReadGps(*exifData, latitude, longitude);
}

bool CPictureMetadataExiv::ReadDateTime(const Exiv2::ExifData& exifData,
	wxString& dateTime)
{
	static const Exiv2::ExifKey key("Exif.Image.DateTime");

	auto it = exifData.findKey(key);
	if (it == exifData.end())
		return false;

	dateTime = toString(*it);

	wxDateTime dt;
	return dt.ParseFormat(dateTime, "%Y:%m:%d %H:%M:%S");
}

bool CPictureMetadataExiv::ReadGpsTag(const Exiv2::ExifData& exifData,
	const char* keyName,
	wxString& value)
{
	Exiv2::ExifKey key(keyName);

	auto it = exifData.findKey(key);

	if (it == exifData.end())
		return false;

	value = it->value().toString();

	return true;
}

bool CPictureMetadataExiv::ReadGps(const Exiv2::ExifData& exifData,
	wxString& latitude,
	wxString& longitude)
{
	wxString latitudeRef;
	wxString longitudeRef;

	if (!ReadGpsTag(exifData,
		"Exif.GPSInfo.GPSLatitudeRef",
		latitudeRef))
		return false;

	if (!ReadGpsTag(exifData,
		"Exif.GPSInfo.GPSLatitude",
		latitude))
		return false;

	if (!ReadGpsTag(exifData,
		"Exif.GPSInfo.GPSLongitudeRef",
		longitudeRef))
		return false;

	if (!ReadGpsTag(exifData,
		"Exif.GPSInfo.GPSLongitude",
		longitude))
		return false;

	latitude = GetGpsfValue(latitude);
	longitude = GetGpsfValue(longitude);

	if (latitude.empty() || longitude.empty())
		return false;

	if (latitudeRef == "S")
		latitude.Prepend("-");

	if (longitudeRef == "W")
		longitude.Prepend("-");

	return true;
}

void CPictureMetadataExiv::ReadExif(Exiv2::ExifData& exifData, std::vector<CMetadata>& metadataList)
{
	Exiv2::ExifData::const_iterator end = exifData.end();

	for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
	{
		CMetadata metadata;
		metadata.key = i->key();
		metadata.value = toString(*i);

		metadataList.push_back(metadata);
	}
}

void CPictureMetadataExiv::ReadXmp(Exiv2::XmpData& xmpData, std::vector<CMetadata>& metadataList)
{
	wxString exifinfos;
	wxString informations;
	bool apple = false;
	auto end = xmpData.end();
	for (auto md = xmpData.begin(); md != end; ++md)
	{
		informations = md->key();
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
}


wxString CPictureMetadataExiv::GetQuickTimeDate(int64_t dateQuicktime)
{
	char message[1024];
	//long timeFrom1970 = dateQuicktime - 2082844800;
	static const unsigned long SecsUntil1970 = 2082844800;

	struct tm MacTime = { 0 };
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

void CPictureMetadataExiv::ReadIpct(Exiv2::IptcData& ipctData, std::vector<CMetadata>& metadataList)
{
	
	wxString exifinfos;
	wxString informations;
	auto end = ipctData.end();
	for (auto md = ipctData.begin(); md != end; ++md)
	{
		informations = md->key();
		exifinfos = md->value().toString();

		if (md->typeId() == Exiv2::TypeId::unsignedByte)
		{
			Exiv2::Value::UniquePtr value = md->getValue();
			if (value.get())
			{
				std::vector<Exiv2::byte> buffer(value->size());
				long size = value->copy(buffer.data(), Exiv2::ByteOrder::invalidByteOrder);
				if (size > 0)
				{
					exifinfos = wxString::FromUTF8(reinterpret_cast<const char*>(buffer.data()),
						static_cast<size_t>(size));
				}
				else
				{
					exifinfos.clear();
				}
			}
			else
			{
				exifinfos.clear();
			}

			CMetadata metadata;
			metadata.key = informations;
			metadata.value = exifinfos;
			metadataList.push_back(metadata);
		}
	}

}


std::vector<CMetadata> CPictureMetadataExiv::GetMetadata()
{
	std::vector<CMetadata> metadataList;
	if (isExif && exif.get() != nullptr)
	{
		try
		{
			Exiv2::ExifData& exifData = exif->exifData();
			Exiv2::IptcData& ipctData = exif->iptcData();
			Exiv2::XmpData& xmpData = exif->xmpData();
			if (!exifData.empty())
			{
				ReadExif(exifData, metadataList);
			}
			if (!ipctData.empty())
			{
				ReadIpct(ipctData, metadataList);
			}
			if (!xmpData.empty())
			{
				ReadXmp(xmpData, metadataList);
			}
		}
		catch(const std::exception& e)
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
		if (data.size() > 0 && data.data() != nullptr)
		{
			Exiv2::ExifKey orientationKey("Exif.Image.Orientation");
			Exiv2::ExifData::const_iterator md = dataIn->findKey(orientationKey);
			if (dataIn->end() != md)
			{
				wxString value = md->value().toString();
				orientation = atoi(value.c_str());
			}

			wxMemoryInputStream cxMemFile(data.data(), data.size());
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
	catch(const std::exception& e)
	{
	}
	return bitmap;
}
