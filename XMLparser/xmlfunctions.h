#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <string>
#include <memory>
#include <vector>
#include <string_view>
#include <string>
#include <algorithm>

using tinyxml2::XMLError;
using tinyxml2::XMLElement;
namespace XmlParser {
    class XmlParserException : public std::exception {
    public:
        explicit XmlParserException(const char* message) noexcept : m_message(message) {}
        explicit XmlParserException(const std::string& message) noexcept : m_message(message) {}
        virtual ~XmlParserException() noexcept {}

        virtual const char* what() const noexcept {
            return m_message.c_str();
        }
    protected:
        std::string m_message{ "Invalid Xml Parser Query" };
    };

    class XMLParser {
    public:
        using XmlElement = tinyxml2::XMLElement;
        using XmlDocument = tinyxml2::XMLDocument;

        XMLParser(const char* path)
        {
            doc.LoadFile(path);
            root = (doc.RootElement());
            if (!is_root_null())
                elementCostumers = (static_cast<XmlElement*> (doc.RootElement()->FirstChild()));
            if (is_root_null())
                throw XmlParserException{ "root is null" };
        }

        void is_open() const {
            if (get_errorID()) {
                throw XmlParserException{ "XML can not open " };
            }
        }
        int get_errorID() const { return doc.ErrorID(); }
        bool is_root_null() {
            return root == nullptr ? true : false;
        }
        bool is_customer_null() const {
            return elementCostumers == nullptr ? true : false;
        }
        template<typename Child>
        std::string is_valid_tag(Child child) {
            return child != nullptr ? child->GetText() : "";
        }

        template<typename Attribute>
        std::string is_valid_attribute(Attribute attribute)
        {
            return attribute != nullptr ? attribute : "";
        }
        void insert_element(std::string ID, std::string name, std::string surname, std::string phone, std::string address) {
            auto newElement = doc.NewElement("customer");
            newElement->SetAttribute("id", ID.c_str());

            auto newListName = doc.NewElement("name");
            newListName->SetText(name.c_str());
            newElement->InsertEndChild(newListName);

            auto newListSurname = doc.NewElement("subname");
            newListSurname->SetText(surname.c_str());
            newElement->InsertEndChild(newListSurname);

            auto newListPhone = doc.NewElement("tel");
            newListPhone->SetText(phone.c_str());
            newElement->InsertEndChild(newListPhone);

            auto newLisAddress = doc.NewElement("address");
            newLisAddress->SetText(address.c_str());
            newElement->InsertEndChild(newLisAddress);

            root->InsertEndChild(newElement);

            XMLError eResult = doc.SaveFile("customerDB.xml");
        }

        XMLElement* find_node(std::string ID, std::string name, std::string subname, std::string phone, std::string address) {
            auto child = root->FirstChildElement();
            while (child != nullptr) {
                if (child->Attribute("id") == ID &&
                    child->FirstChildElement("name")->GetText() == name &&
                    child->FirstChildElement("subname")->GetText() == subname &&
                    child->FirstChildElement("tel")->GetText() == phone &&
                    child->FirstChildElement("address")->GetText() == address)
                    break;
                else
                    child = child->NextSiblingElement();

            }
            return child;
        }

        void delete_node(std::string ID, std::string name, std::string subname, std::string phone, std::string address) {
            auto child = find_node(ID, name, subname, phone, address);
            root->GetDocument()->DeleteNode(child);
            XMLError eResult = doc.SaveFile("customerDB.xml");
        }

        void replace_node(std::string oldID, std::string oldname, std::string oldsubname, std::string oldphone, std::string oldaddress,
            std::string newID, std::string newname, std::string newsubname, std::string newphone, std::string newaddress) {
            auto child = find_node(oldID, oldname, oldsubname, oldphone, oldaddress);
            child->SetAttribute("id", newID.c_str());
            child->FirstChildElement("name")->SetText(newname.c_str());
            child->FirstChildElement("subname")->SetText(newsubname.c_str());
            child->FirstChildElement("tel")->SetText(newphone.c_str());
            child->FirstChildElement("address")->SetText(newaddress.c_str());
            XMLError eResult = doc.SaveFile("customerDB.xml");
        }
    private:
        XmlDocument doc;
        XmlElement* elementCostumers;
        XmlElement* root;
    };
}