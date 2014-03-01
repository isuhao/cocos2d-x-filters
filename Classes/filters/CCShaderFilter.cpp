#include "CCShaderFilter.h"

CCShaderFilter::CCShaderFilter()
: vertex(NULL)
, fragment(NULL)
, shaderName(NULL)
, _pAttributes(NULL)
, _pParameters(NULL)
{

}


CCShaderFilter::~CCShaderFilter()
{
}

CCArray* CCShaderFilter::getAttributes()
{
	return _pAttributes;
}

CCArray* CCShaderFilter::getParameters()
{
	return _pParameters;
}

void CCShaderFilter::addAttribute(GLint $value, const char* $key)
{
	if (!_pAttributes)
	{
		_pAttributes = CCArray::createWithCapacity(4);
	}
	addDictionary($value, $key, _pAttributes);
}

void CCShaderFilter::addParameter(GLint $value, const char* $key)
{
	if (!_pParameters)
	{
		_pParameters = CCArray::createWithCapacity(4);
	}
	addDictionary($value, $key, _pParameters);
}

void CCShaderFilter::addDictionary(GLint $value, const char* $key, CCArray* $arr)
{
	CCDictionary* __dict = CCDictionary::create();
	__dict->setObject(CCString::create($key), "name");
	__dict->setObject(CCInteger::create($value), "index");
	$arr->addObject(__dict);
}

CCGLProgram* CCShaderFilter::getProgram()
{
	CCGLProgram* pProgram = CCShaderCache::sharedShaderCache()->programForKey(shaderName);
	CCLOG("CCShaderFilter::getProgram %s", shaderName);
	if (!pProgram)
	{
		pProgram = new CCGLProgram();

		pProgram->initWithVertexShaderByteArray(this->vertex, this->fragment);

		for (size_t i = 0; i < this->getAttributes()->count(); i++)
		{
			CCDictionary* __dict = static_cast<CCDictionary*>(this->getAttributes()->objectAtIndex(i));
			pProgram->addAttribute(__dict->valueForKey("name")->getCString(),
				static_cast<CCInteger*>(__dict->objectForKey("index"))->getValue());

			CCLOG("CCShaderFilter::getProgram %s, %d", __dict->valueForKey("name")->getCString(), static_cast<CCInteger*>(__dict->objectForKey("index"))->getValue());
		}
		CCShaderCache::sharedShaderCache()->addProgram(pProgram, this->shaderName);
	}
	return pProgram;
}

CCGrayFilter* CCGrayFilter::create()
{
	CCGrayFilter* __filter = new CCGrayFilter();
	__filter->autorelease();
	return __filter;
}

CCGrayFilter* CCGrayFilter::create(ccColor4F $param)
{
	CCGrayFilter* __filter = CCGrayFilter::create();
	__filter->setParameter($param);
	return __filter;
}

CCGrayFilter::CCGrayFilter()
: _pParam(ccc4f(0.299, 0.587, 0.114, 0.0f))
{
	this->vertex = ccFilterShader_gray_vert;
	this->fragment = ccFilterShader_gray_frag;
	this->shaderName = kCCFilterShader_gray;

	this->addAttribute(kCCVertexAttrib_Position, kCCAttributeNamePosition);
	this->addAttribute(kCCVertexAttrib_Color, kCCAttributeNameColor);
	this->addAttribute(kCCVertexAttrib_TexCoords, kCCAttributeNameTexCoord);
	this->addAttribute(kCCVertexAttrib_grayParam, kCCAttributeName_grayParam);

	this->addParameter(kCCVertexAttrib_grayParam, kCCAttributeName_grayParam);
}

void CCGrayFilter::draw()
{
	//gray filter
	GLfloat __param[] = { _pParam.r, _pParam.g, _pParam.b, _pParam.a};
	glVertexAttrib4fv(kCCVertexAttrib_grayParam, __param);
}


void CCGrayFilter::setParameter(ccColor4F $param)
{
	_pParam = $param;
}

CCGrayFilter::~CCGrayFilter()
{
	
}


CCBlurFilter* CCBlurFilter::create()
{
	CCBlurFilter* __filter = new CCBlurFilter();
	__filter->autorelease();
	return __filter;
}

CCBlurFilter::CCBlurFilter()
{
	this->vertex = ccPositionTextureA8Color_vert;
	this->fragment = ccFilterShader_blur_frag;
	this->shaderName = kCCFilterShader_blur;

	this->addAttribute(kCCVertexAttrib_Position, kCCAttributeNamePosition);
	this->addAttribute(kCCVertexAttrib_TexCoords, kCCAttributeNameTexCoord);
}

CCGLProgram* CCBlurFilter::getProgram()
{
	CCGLProgram* __program = CCShaderFilter::getProgram();
	int __fade = glGetUniformLocation(__program->getProgram(), "fade");
	glUniform1f(__fade, 1.0f);
	return __program;
}

void CCBlurFilter::draw()
{
	CCLOG("DO NOTHING!");
}

CCBlurFilter::~CCBlurFilter()
{
}