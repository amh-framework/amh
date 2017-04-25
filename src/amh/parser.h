/*
  Copyright (C) Dolphin project-team, CRIStAL, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
 */

#pragma once

#include <iostream>
#include <iomanip>
#include <string>
// todo: check if sufficient

namespace amh {
  class parser_param_base {
  public:
    parser_param_base(std::string s) : name(s) {}
    virtual ~parser_param_base() {}

    std::string get_name() {
      return name;
    }

    bool responds_to(std::string s) {
      if (name == s)
        return true;
      for (auto& a : aliases) {
        if (a == s)
          return true;
      }
      return false;
    }

    parser_param_base* add_alias() {
      return this;
    }
    template<class ...TT>
    parser_param_base* add_alias(std::string s, TT...ss) {
      aliases.push_back(s);
      return add_alias(ss...);
    }

    parser_param_base* set_default(std::string s) {
      default_value = s;
      has_default_value = true;
      if (!has_value) {
        value = s;
        has_value = true;
      }
      return this;
    }

    virtual parser_param_base* set_implicit(std::string s) {
      implicit_value = s;
      has_implicit_value = true;
      return this;
    }

    virtual void use_implicit() {
      if (has_implicit_value)
        value = implicit_value;
      throw std::runtime_error("amh::parser_param: no implicit value for this parameter");
    }

    std::string get_value() {
      if (has_value)
        return value;
      else if (has_default_value)
        return default_value;
      throw std::runtime_error("amh::parser_param: no value");
    }

    virtual void read_value(char* s) {
      value = s;
    }

    virtual void read_value(std::string s) {
      value = s;
    }

    virtual void set_value(std::string s) {
      value = s;
    }

    static bool read_bool(std::string s) {
      std::transform(s.begin(), s.end(), s.begin(), ::tolower);
      if (s == "1" || s == "true")
        return true;
      if (s == "0" || s == "false")
        return false;
      throw std::runtime_error("amh::parser_param: illegal boolean value");
    }

    void set_user_defined(bool b) {
      user_defined = b;
    }

    bool is_user_defined() {
      return user_defined;
    }

    virtual parser_param_base* set_description(std::string s) {
      description = s;
      return this;
    }

    virtual parser_param_base* set_default_description(std::string s) {
      default_description = s;
      return this;
    }

    virtual parser_param_base* set_implicit_description(std::string s) {
      implicit_description = s;
      return this;
    }

    void set_unknown(const bool b) {
      unknown = b;
    }

    bool is_unknown() {
      return unknown;
    }

    virtual void print_help_on(std::ostream& out, int pad) {
      out << "you should never read that" << std::endl;
    }

    virtual void print_value_on(std::ostream& out, int pad) {
      out << "you should never read that" << std::endl;
    }

  protected:
    std::string name;
    std::string description;
    std::vector<std::string> aliases;
    std::string value;
    std::string default_value;
    std::string implicit_value;
    std::string default_description;
    std::string implicit_description;
    bool has_value = false;
    bool has_default_value = false;
    bool has_implicit_value = false;
    bool user_defined = false;
    bool unknown = true;
  };

  template<class T>
  class parser_param : public parser_param_base {
  public:
    parser_param(std::string s, T v) : parser_param_base(s) {
      set_value(v);
      set_default(v);
    }
    parser_param(std::string s, T v, T iv) : parser_param_base(s) {
      set_value(v);
      set_default(v);
      set_implicit(iv);
    }

    parser_param* set_default(T v) {
      default_value = v;
      has_default_value = true;
      if (!has_value) {
        value = v;
        has_value = true;
      }
      return this;
    }

    parser_param* set_implicit(T v) {
      implicit_value = v;
      has_implicit_value = true;
      return this;
    }

    void use_implicit() {
      if (has_implicit_value)
        value = implicit_value;
    }

    T get_value() {
      if (has_value)
        return value;
      else if (has_default_value)
        return default_value;
      throw std::runtime_error("no value");
    }

    void read_value(char* s) {
      std::istringstream is(s);
      if (std::is_same<T,bool>::value)
        value = read_bool(s);
      else
        is >> value;
    }

    void read_value(std::string s) {
      std::istringstream is(s);
      if (std::is_same<T,bool>::value)
        value = read_bool(s);
      else
        is >> value;
    }

    void set_value(T v) {
      value = v;
    }

    parser_param* set_description(std::string s) {
      description = s;
      return this;
    }

    parser_param* set_default_description(std::string s) {
      default_description = s;
      return this;
    }

    parser_param* set_implicit_description(std::string s) {
      implicit_description = s;
      return this;
    }

    void print_help_on(std::ostream& out, int pad) {
      std::stringstream ss;
      int k = name.size()+2;
      ss << "  " << name;
      if (aliases.size() > 0) {
        for (auto& a : aliases) {
          ss << ", " << a;
          k += a.size()+2;
        }
      }
      if (k < pad-2)
        out << std::left << std::setw(pad) << std::setfill(' ') << ss.str();
      else
        out << ss.str() << std::endl << std::string(pad, ' ');
      if (description.size() > 0) {
        ss.str(description);
        std::string tmp;
        bool first = true;
        while (std::getline(ss, tmp, '\n')) {
          if (first)
            first = false;
          else
            out << std::string(pad, ' ');
          out << "# " << tmp << std::endl;
        }
      } else {
        out << std::endl;
      }
    }

    void print_value_on(std::ostream& out, int pad) {
      std::stringstream ss;
      if (user_defined) {
        ss << name << "=" << get_value();
        if (has_implicit_value) {
          out << std::left << std::setw(pad) << std::setfill(' ') << ss.str() << " ";
          if (implicit_description != "")
            out << "# implicit value: " << implicit_description;
          else
            out << "# implicit value";
        } else {
          out << ss.str();
        }
      } else {
        ss << "# " << name << "=" << get_value();
        if (default_description != "") {
          out << std::left << std::setw(pad) << std::setfill(' ') << ss.str() << " ";
          out << "# default value: " << default_description;
        } else {
          out << ss.str();
        }
      }
      out << std::endl;
    }

  protected:
    T value;
    T default_value;
    T implicit_value;
  };

  class parser {
  public:
    parser() {}
    ~parser() {
      for (auto& p : params)
        delete p;
    }

    void parse(int argc, char* argv[]) {
      parser_param_base *lastp = NULL;
      for (int i=1; i<argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
          if (arg.length() == 1)
            throw std::runtime_error("amh::parser: what it parameter `-'?");
          if (arg[1] == '-' || !grouping) { // --param... (or -param without grouping)
            if (lastp) {
              lastp->use_implicit();
              lastp = NULL;
            }
            int k;
            if ((k = arg.find("=")) != std::string::npos) { // --param=...
              std::string tmp = arg.substr(0, k);
              if (lower_params)
                std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
              if (has_param(tmp)) {
                auto p = get_param(tmp);
                p->read_value(arg.substr(k+1, std::string::npos));
                p->set_user_defined(true);
              } else {
                unknown_params.push_back(arg);
                auto p = new parser_param<std::string>(arg, "");
                p->read_value(arg.substr(k+1, std::string::npos));
                p->set_unknown(true);
                params.push_back(p);
              }
            } else { // --param
              if (lower_params)
                std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
              if (has_param(arg)) {
                lastp = get_param(arg);
                lastp->set_user_defined(true);
              } else {
                unknown_params.push_back(arg);
                auto p = new parser_param<std::string>(arg, "");
                p->set_unknown(true);
                p->set_user_defined(true);
                params.push_back(p);
                lastp = p;
              }
            }
          } else { // -param... (grouping)
            if (lastp) {
              lastp->use_implicit();
              lastp = NULL;
            }
            std::string tmp;
            for (int k=1; k<arg.length(); k++) {
              if (arg[k] == '=') {
                if (k == 1)
                  throw std::runtime_error("amh::parser: what is `-='?");
                if (k != 2)
                  throw std::runtime_error("amh::parser: no `=' allowed in aggregated single `-' param");
                tmp = "-";
                tmp += arg[1];
                if (has_param(tmp)) {
                  auto p = get_param(tmp);
                  p->set_user_defined(true);
                  p->read_value(arg.substr(3, std::string::npos));
                } else {
                  unknown_params.push_back(tmp);
                  auto p = new parser_param<std::string>(tmp, "");
                  p->set_unknown(true);
                  params.push_back(p);
                }
                break;
              } else if (k == arg.length()-1 || arg[k+1] != '=') {
                tmp = "-";
                tmp += arg[k];
                if (has_param(tmp)) {
                  if (arg.length() == 2) {
                    lastp = get_param(tmp);
                    lastp->set_user_defined(true);
                  } else {
                    auto p = get_param(tmp);
                    p->use_implicit();
                    p->set_user_defined(true);
                 }
                } else {
                  unknown_params.push_back(tmp);
                  auto p = new parser_param<std::string>(tmp, "");
                  p->set_unknown(true);
                  params.push_back(p);
                }
              }
            }
          }
        } else if (lastp) {
          lastp->read_value(argv[i]);
          lastp = NULL;
        } else {
          free_params.push_back(arg);
        }
      }
      if (lastp)
        lastp->use_implicit();
    }

    template <class T, class ...TT>
    parser_param<T>* set_as(std::string name, T value, TT...ss) {
      auto p = new parser_param<T>(name, value);
      p->add_alias(ss...);
      p->set_unknown(false);
      params.push_back(p);
      return p;
    }

    bool has_param(std::string name) {
      for (auto& p : params)
        if (p->responds_to(name))
          return true;
      return false;
    }

    bool is_user_defined(std::string name) {
      for (auto& p : params)
        if (p->responds_to(name))
          return p->is_user_defined();
      return false;
    }

    bool got_unknown_params() {
      return unknown_params.size() > 0;
    }

    std::vector<std::string> get_unknown_params() {
      return unknown_params;
    }

    parser_param_base* get_param(std::string name) {
      for (auto& p : params)
        if (p->responds_to(name))
          return p;
      throw std::runtime_error("amh::parser: no parameter found");
    }

    bool got_free_params() {
      return free_params.size() > 0;
    }

    std::vector<std::string> get_free_params() {
      return free_params;
    }

    template <class T>
    T get_as(std::string name) {
      auto p = get_param(name);
      return dynamic_cast<parser_param<T>*>(p)->get_value();
    }

    template <class T>
    T get_free_as(int k) {
      std::istringstream is(free_params[k]);
      T value;
      is >> value;
      return value;
    }

    bool get_free_as_bool(int k) {
      std::string tmp = free_params[k];
      return parser_param_base::read_bool(tmp);
    }

    void printHelp(std::ostream& out) {
      for (auto& p : params)
        if (!p->is_unknown())
          p->print_help_on(out, 40);
    }

    void printConfig(std::ostream& out) {
      for (auto& p : params)
          p->print_value_on(out, 40);
    }

    bool lower_params = false;
    bool grouping = false;
  protected:
    std::vector<parser_param_base*> params;
    std::vector<std::string> free_params;
    std::vector<std::string> unknown_params;
  };
}
