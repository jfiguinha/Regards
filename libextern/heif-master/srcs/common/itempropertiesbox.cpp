/* This file is part of Nokia HEIF library
 *
 * Copyright (c) 2015-2021 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: heif@nokia.com
 *
 * This software, including documentation, is protected by copyright controlled by Nokia Corporation and/ or its
 * subsidiaries. All rights are reserved.
 *
 * Copying, including reproducing, storing, adapting or translating, any or all of this material requires the prior
 * written consent of Nokia.
 */

#include "itempropertiesbox.hpp"

#include "hevcconfigurationbox.hpp"
#include "imagespatialextentsproperty.hpp"
#include "log.hpp"

ItemPropertiesBox::ItemPropertiesBox()
    : Box("iprp")
{
}

const Box* ItemPropertiesBox::getPropertyByIndex(const std::uint32_t index) const
{
    return mContainer.getProperty(index);
}

std::uint32_t ItemPropertiesBox::findPropertyIndex(const PropertyType type, const std::uint32_t itemId) const
{
    for (const auto& ipma : mAssociationBoxes)
    {
        const ItemPropertyAssociation::AssociationEntries& propertyIndexVector = ipma.getAssociationEntries(itemId);
        for (const auto& entry : propertyIndexVector)
        {
            const Box* itemproperty = mContainer.getProperty(static_cast<size_t>(entry.index - 1));
            if (itemproperty && getPropertyType(itemproperty) == type)
            {
                return entry.index;
            }
        }
    }
    return 0;
}

Vector<std::uint8_t> ItemPropertiesBox::getPropertyDataByIndex(std::uint32_t index) const
{
    BitStream serialized;
    const Box* property = mContainer.getProperty(index);
    if (property != nullptr)
    {
        property->writeBox(serialized);
    }

    return serialized.getStorage();
}

ItemPropertiesBox::PropertyType ItemPropertiesBox::getPropertyType(const Box* property) const
{
    const FourCCInt boxType = property->getType();
    const Map<FourCCInt, PropertyType> TYPE_MAPPING{
        {"altt", PropertyType::ALTT}, {"auxC", PropertyType::AUXC}, {"avcC", PropertyType::AVCC},
        {"clap", PropertyType::CLAP}, {"colr", PropertyType::COLR}, {"crtt", PropertyType::CRTT},
        {"free", PropertyType::FREE}, {"hvcC", PropertyType::HVCC}, {"imir", PropertyType::IMIR},
        {"irot", PropertyType::IROT}, {"iscl", PropertyType::ISCL}, {"ispe", PropertyType::ISPE},
        {"jpgC", PropertyType::JPGC}, {"mdft", PropertyType::MDFT}, {"pasp", PropertyType::PASP},
        {"pixi", PropertyType::PIXI}, {"rloc", PropertyType::RLOC}, {"rref", PropertyType::RREF},
        {"skip", PropertyType::FREE}, {"udes", PropertyType::UDES},
    };

    const auto found = TYPE_MAPPING.find(boxType);
    if (found == TYPE_MAPPING.end())
    {
        return PropertyType::RAW;
    }

    return found->second;
}

ItemPropertiesBox::PropertyInfos ItemPropertiesBox::getItemProperties(const std::uint32_t itemId) const
{
    PropertyInfos propertyInfoVector;
    for (const auto& ipma : mAssociationBoxes)
    {
        const ItemPropertyAssociation::AssociationEntries& associations = ipma.getAssociationEntries(itemId);

        for (const auto& entry : associations)
        {
            if (entry.index == 0)
            {
                // Index value 0 indicates no property is associated.
                continue;
            }

            PropertyInfo propertyInfo;
            const Box* itemproperty = mContainer.getProperty(static_cast<size_t>(entry.index - 1));
            if (itemproperty)
            {
                propertyInfo.type = getPropertyType(itemproperty);
                if (propertyInfo.type == PropertyType::FREE)
                {
                    // Ignore a FreeSpaceBox property. It should not have any associations.
                    continue;
                }
                propertyInfo.index     = static_cast<std::uint32_t>(entry.index - 1);
                propertyInfo.essential = entry.essential;
                propertyInfoVector.push_back(propertyInfo);
            }
            else
            {
                throw RuntimeError("ItemPropertiesBox::getItemProperties() invalid property index");
            }
        }
        if (associations.size() > 0)
        {
            break;
        }
    }

    return propertyInfoVector;
}

uint16_t ItemPropertiesBox::addProperty(std::shared_ptr<Box> box,
                                        const Vector<std::uint32_t>& itemIds,
                                        const bool essential)
{
    const std::uint16_t propertyIndex = mContainer.addProperty(std::move(box));
    associateProperty(propertyIndex, itemIds, essential);
    return propertyIndex;
}

void ItemPropertiesBox::associateProperty(const std::uint16_t index,
                                          const Vector<std::uint32_t>& itemIds,
                                          const bool essential)
{
    if (mAssociationBoxes.size() == 0)
    {
        ItemPropertyAssociation ipma;
        mAssociationBoxes.push_back(ipma);
    }
    if (mAssociationBoxes.size() != 1)
    {
        // Writing multiple ipma boxes is not required by upper layers but would require extra code to handle it.
        throw RuntimeError("ItemPropertiesBox: Writing multiple ItemPropertyAssociation boxes is not supported.");
    }
    for (const auto itemId : itemIds)
    {
        mAssociationBoxes.at(0).addEntry(itemId, index, essential);
    }
}

void ItemPropertiesBox::writeBox(BitStream& output) const
{
    writeBoxHeader(output);

    mContainer.writeBox(output);

    if (mAssociationBoxes.size() > 1)
    {
        // Writing multiple ipma boxes is not required by upper layers but would require extra code to handle it.
        throw RuntimeError("ItemPropertiesBox: Writing multiple ItemPropertyAssociation boxes is not supported.");
    }
    else if (mAssociationBoxes.size() == 0)
    {
        ItemPropertyAssociation ipma;
        ipma.writeBox(output);
    }
    else
    {
        mAssociationBoxes.at(0).writeBox(output);
    }

    updateSize(output);
}

void ItemPropertiesBox::parseBox(BitStream& input)
{
    parseBoxHeader(input);

    FourCCInt subBoxType;
    BitStream subBitStream = input.readSubBoxBitStream(subBoxType);
    mContainer.parseBox(subBitStream);

    while (input.numBytesLeft() > 0)
    {
        subBitStream = input.readSubBoxBitStream(subBoxType);
        if (subBoxType != "ipma")
        {
            throw RuntimeError("ItemPropertiesBox includes a box which is not ipma");
        }
        ItemPropertyAssociation ipma;
        ipma.parseBox(subBitStream);
        mAssociationBoxes.push_back(ipma);
    }
}
