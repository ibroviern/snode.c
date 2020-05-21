#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "httputils.h"

#include "Response.h"
#include "HTTPContext.h"
#include "HTTPStatusCodes.h"


Response::Response(HTTPContext* httpContext) : httpContext(httpContext) {}


void Response::status(int status) const {
    this->httpContext->responseStatus = status;
}

/*
void Response::set(const std::string& field, const std::string& value) const {
    this->httpContext->responseHeader.insert({field, value});
}
*/

void Response::set(const std::multimap<std::string, std::string>& map) const {
    this->httpContext->responseHeader.merge(const_cast<std::multimap<std::string, std::string>&>(map));
}


void Response::cookie(const std::string& name, const std::string& value, const std::map<std::string, std::string>& options) const {
    this->httpContext->responseCookies.insert({name, ResponseCookie(value, options)});
}


void Response::clearCookie(const std::string& name, const std::map<std::string, std::string>& options) const {
    std::map<std::string, std::string> opts = options;
    
    opts.erase("Max-Age");
    time_t time = 0;
    opts["Expires"] = httputils::to_http_date(gmtime(&time));
    
    this->cookie(name, "", opts);
}


void Response::send(const char* puffer, int n) const {
    this->httpContext->send(puffer, n);
}


void Response::send(const std::string& text) const {
    this->httpContext->send(text);
}


void Response::sendFile(const std::string& file, const std::function<void (int err)>& fn) const {
    this->httpContext->sendFile(file, fn);
}


void Response::download(const std::string& file, const std::function<void (int err)>& fn) const {
    std::string name = file;
    
    if (name[0] == '/') {
        name.erase(0, 1);
    }
    
    this->download(file, name, fn);
}


void Response::download(const std::string& file, const std::string& name, const std::function<void (int err)>& fn) const {
    this->set({{"Content-Disposition", "attachment; filename=\"" + name + "\""}});
    this->sendFile(file, fn);
}


void Response::redirect(const std::string& name) const {
    this->redirect(302, name);
}


void Response::redirect(int status, const std::string& name) const {
    this->status(status);
    this->set({{"Location", name}});
    this->end();
}


void Response::sendStatus(int status) const {
    this->status(status);
    this->send(HTTPStatusCode::reason(status));
}


void Response::type(std::string type) const {
    this->set({{"Content-Type", type}});
}


void Response::end() const {
    this->httpContext->end();
}
