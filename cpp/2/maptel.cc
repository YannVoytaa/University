#include <iostream>
#include <unordered_map>
#include <regex>
#include <cassert>
#include "maptel.h"
using namespace std;

namespace {
  using TelGraph = unordered_map<string, string>;
  using IdType = unsigned long;
  using TelGraphMap = unordered_map<IdType, TelGraph>;
  using VisitedNumbersMap = unordered_map<string, bool>;

  regex& getTelephoneValidationRegex() {
    static regex pattern("^\\d+$");
    return pattern;
  }

  VisitedNumbersMap& getVisitedNumbers() {
    static VisitedNumbersMap visitedNumbers;
    return visitedNumbers;
  }

  TelGraphMap& getMapList() {
    static TelGraphMap mapList;
    return mapList;
  }

  template<class... Args>
  void log([[maybe_unused]] Args... args) {
    #ifndef NDEBUG
      ios_base::Init();
      (cerr << "maptel: " << ... << args) << "\n";
    #endif
  }

  bool checkIfMapExists(TelGraphMap::iterator& it) {
    if (it == getMapList().end()) {
      log("map doesn't exist");
      return false;
    }
    return true;
  }

  bool telIsValid(const char *tel) {
    if (tel == NULL) {
      log("pointer is NULL");
      return false;
    }
    if (strlen(tel) > jnp1::TEL_NUM_MAX_LEN) {
      log("tel number is too long");
      return false;
    }
    if (tel[strlen(tel)] != '\0') {
      log("tel doesn't end with \\0");
      return false;
    }
    if (!regex_match(tel, getTelephoneValidationRegex())) {
      log("tel contains invalid characters");
      return false;
    }
    return true;
  }

  string getAssignedNumber(TelGraph& map, char const *first) {
    getVisitedNumbers().clear();
    string firstNumber(first);
    string act = firstNumber;
    getVisitedNumbers()[act] = true;
    while (map.find(act) != map.end()) {
      act = map[act];
      if (getVisitedNumbers()[act]) {
        log("maptel_transform: cycle detected");
        return firstNumber;
      }
      getVisitedNumbers()[act] = true;
    }
    return act;
  }

  bool myAssert(bool condition) {
    assert(condition);
    return condition;
  }
}

namespace jnp1 {

  IdType maptel_create() {
    static IdType maxId = 0;
    log("maptel_create()");
    //making sure map is initialized and mapList.find(id) returns true
    getMapList()[maxId][(char *)""] = (char *)"";
    log("maptel_create: new map id = ", maxId);

    return maxId++;
  }

  void maptel_insert(IdType id, char const* tel_src, char const* tel_dst) {
    TelGraphMap::iterator it = getMapList().find(id);
    if (
      !myAssert(checkIfMapExists(it)) ||
      !myAssert(telIsValid(tel_src) && telIsValid(tel_dst))
    ) {
      return;
    }
    log("maptel_insert(", id, ", ", tel_src, ", ", tel_dst, ")");
    string src(tel_src);
    string dst(tel_dst);
    it->second[src] = dst;
    log("maptel_insert: inserted");
  }

  void maptel_delete(IdType id) {
    TelGraphMap::iterator it = getMapList().find(id);
    if (!myAssert(checkIfMapExists(it))) {
      return;
    }

    log("maptel_delete(", id, ")");
    getMapList().erase(it);
    log("maptel_delete: map ", id, " deleted");
  }

  void maptel_erase(IdType id, char const *tel_src) {
    TelGraphMap::iterator it = getMapList().find(id);
    if (
      !myAssert(checkIfMapExists(it)) ||
      !myAssert(telIsValid(tel_src))
    ) {
      return;
    }

    log("maptel_erase(", id, ", ", tel_src, ")");
    string src(tel_src);
    if (it->second.find(src) == it->second.end()) {
      log("maptel_erase: nothing to erase");
      return;
    }
    it->second.erase(src);
    log("maptel_erase: erased");
  }

  void maptel_transform(IdType id, char const *tel_src, char *tel_dst, size_t len) {
    TelGraphMap::iterator it = getMapList().find(id);
    if (
      !myAssert(tel_dst != NULL) ||
      !myAssert(checkIfMapExists(it)) ||
      !myAssert(telIsValid(tel_src))
    ) {
      return;
    }

    log("maptel_transform(", id, ", ", tel_src, ", ", &tel_dst, ", ", len, ")");
    string number = getAssignedNumber(it->second, tel_src);
    if (!myAssert(number.size() < len)) {
      return;
    }
    strcpy(tel_dst, number.c_str());
    log("maptel_transform: ", tel_src, " -> ", number);
  }
}
