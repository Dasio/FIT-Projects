#include "request_parser.h"
#include "common.h"
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "boost/filesystem.hpp"

const std::string RequestParser::doc = "www";
const std::string RequestParser::img_doc = "/icons/image1.gif";
const std::string RequestParser::img_img = "/icons/image2.gif";
const std::string RequestParser::img_dir = "/icons/folder.gif";
void RequestParser::parse()
{
    for(auto c: data)
    {
        readpos++;
        if(c != ' ')
            method += c;
        else
            break;
    }
    for(;data[readpos] != ' ';readpos++)
    {
        path += data[readpos];
    }
    boost::iterator_range<std::string::iterator> range = boost::algorithm::ifind_first(data,"Connection: Keep-Alive");
    if(!range.empty())
        keepAlive = true;
}

reply RequestParser::handle()
{
    bool index = false;
    path = doc + path;
    std::string decoded_path;
    if(!url_decode(path,decoded_path))
        return reply::stock_reply(reply::bad_request);
    path = decoded_path;
    reply rep;
    rep.status = reply::ok;
    std::string mime;
    boost::filesystem::path p(path);
    /* If path is directory return list */
    if(is_directory(p))
    {
        if(boost::filesystem::exists(path + "/index.html"))
        {
            path += "/index.html";
        }
        else
        {
            rep.content = gen_list(path);
            index = true;
        }
        mime = "text/html";

    }
    /* Else it is file, open file and write it to content */
    if(!index)
    //else
    {
        std::ifstream is(path, std::ios::in | std::ios::binary);
        if(!is)
            return reply::stock_reply(reply::not_found);
        char buf[512];
        while (is.read(buf, sizeof(buf)).gcount() > 0)
            rep.content.append(buf, is.gcount());
        if(mime.empty())
            mime = ext2mime(p.extension().string());
    }

    rep.headers.push_back({"Content-Length",std::to_string(rep.content.size())});
    rep.headers.push_back({"Content-Type",mime});
    if(keepAlive)
        rep.headers.push_back({"Connection","Keep-Alive"});
    return rep;

}

std::string RequestParser::ext2mime(std::string ext)
{
    if(ext == ".gif")
        return "image/gif";
    else if (ext == ".jpg")
        return "image/jpeg";
    else if (ext == ".png")
        return "image/png";
    else if (ext == ".htm" || ext == ".html")
        return "text/html";
    else
        return "text/plain";
}
std::string RequestParser::gen_list(std::string path)
{
    std::string pathx = path.substr(doc.length());
    boost::filesystem::path p = boost::filesystem::system_complete(path);
    std::string html = "<html><head><title> Index of " + pathx + "</title></head><body><h1>Index of " + pathx +
    "<table><tr><th><img src=\"/icons/blank.gif\" alt = \"[ICO]\"></th><th>";
    html += "Name</a></th></tr>";
    html += "<tr><td valign=\"top\"><img src=\"/icons/back.gif\" alt=\"[DIR]\"></td><td><a href=\"/\">Parent Directory</a> ";

    for(auto &entry: boost::make_iterator_range(boost::filesystem::directory_iterator(p), {}))
    {
        html += "<tr><td valign=\"top\"><img src=";
        if(is_directory(entry.path()))
            html += "\"" + img_dir + "\" alt=\"[DIR]\"></td><td><a href=";
        else
        {
            std::string ext = entry.path().extension().string();
            if(ext == ".jpg" || ext == ".png")
                html += "\"" + img_img + "\" alt=\"[IMG]\"></td><td><a href=";
            else
                html += "\"" + img_doc + "\" alt=\"[DOC]\"></td><td><a href=";
        }
        html += entry.path().filename().string() + ">" + entry.path().filename().string() + "</a></td>";

    }

    html += "</table><address>IPK 2015 Server(xmikus15) at localhost</address></body></html>";
    return html;
}
