#ifndef OUTPUTITEM_HPP
#define OUTPUTITEM_HPP

#include "JSON/JsonValue.hpp"
#include "output/OutputBase.hpp"
#include <string>

class OutputItem : public OutputBase {
	private:
		void loadType(JsonValue&) override;

	public:
		OutputItem(const std::string&, JsonValue&);
		~OutputItem();
};

#endif