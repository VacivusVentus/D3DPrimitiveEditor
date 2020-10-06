// ---------------------------------------------------------------------------

#ifndef XMLConverterH
#define XMLConverterH
#include <stdio.h>
#include "Primitive.h"
#include "PrimitiveManager.h"

// ---------------------------------------------------------------------------
class XMLConverter {
public:
	XMLConverter(char* filename, bool save = true);
	~XMLConverter();

	bool isInit() const {
		return correct;
	}
	void __fastcall saveElement(Primitive *primitive);
	void __fastcall parseDocument(PrimitiveManager *primitiveManager);
private:
	bool correct;
    bool save;
	FILE *file;
	std::string name;
	protected:
    float __fastcall getFloatFromWString(std::wstring);
};
#endif
