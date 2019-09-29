//
// Created by 林炳河 on 2019/7/14.
//

#include "XFilter.hpp"
#include "XImage.hpp"
#include "XLog.hpp"
#include "XFrameBufferPool.hpp"

NS_X_IMAGE_BEGIN
//顶点以及纹理坐标对象
struct PosTexVertex
{
    float m_x;
    float m_y;
    float m_z;
    float t_x;
    float t_y;

    static void init()
    {
        ms_decl
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0,   2, bgfx::AttribType::Float)
                .end();
    };

    static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosTexVertex::ms_decl;
//顶点坐标数据
static PosTexVertex s_vertices[] =
        {
                {-1.0f,  1.0f,  0.0f, 0.0f, 0.0f },
                { 1.0f,  1.0f,  0.0f, 1.0f, 0.0f },
                {-1.0f, -1.0f,  0.0f, 0.0f, 1.0f },
                { 1.0f, -1.0f,  0.0f, 1.0f, 1.0f },
        };
//顶点顺序
static const uint16_t s_triList[] =
        {
                0, 1, 2, // 0
                1, 3, 2,
        };

XFilter::XFilter(std::string vertex, std::string fragment) {
    mVertexShaderPath = vertex;
    mFragmentShaderPath = fragment;
    mProgram = BGFX_INVALID_HANDLE;
    mVertexBuffer = BGFX_INVALID_HANDLE;
    mIndexBuffer = BGFX_INVALID_HANDLE;
    mOutputWidth = 0;
    mOutputHeight = 0;
    mViewMatrix = nullptr;
    mProjectionMatrix = nullptr;
}

XFilter::~XFilter() {
    XImage::destroy(mProgram);
    XImage::destroy(mVertexBuffer);
    XImage::destroy(mIndexBuffer);
    XImage::destroy(mTexture);

    auto iter = mParamHandles.begin();
    while (iter != mParamHandles.end()) {
        XImage::destroy(iter->second);
        iter++;
    }
    mParamHandles.clear();
}

void XFilter::init() {
    int width = mRect.width;
    int height = mRect.height;
    if (mOutputWidth > 0 && mOutputHeight > 0) {
        width = mOutputWidth;
        height = mOutputHeight;
    }
    if (!mTargets.empty() && !mOuterBuffer
            && (mOutputFrameBuffer == nullptr || !mOutputFrameBuffer->isSameSize(width, height))) {
        XFrameBufferPool::recycle(mOutputFrameBuffer);
        mOutputFrameBuffer = XFrameBufferPool::get(width, height);
    }
}

void XFilter::setInputFrameBuffer(XFrameBuffer *input) {
    mFirstInputFrameBuffer = input;
}

void XFilter::setTransform(float view[], float projection[]) {
    if (mViewMatrix = nullptr) {
        mViewMatrix = new float[16]{0};
    }
    if (mProjectionMatrix == nullptr) {
        mProjectionMatrix = new float[16]{0};
    }
    mViewMatrix[0] = view[0];
    mViewMatrix[1] = view[1];
    mViewMatrix[2] = view[2];
    mViewMatrix[3] = view[3];
    mViewMatrix[4] = view[4];
    mViewMatrix[5] = view[5];
    mViewMatrix[6] = view[6];
    mViewMatrix[7] = view[7];
    mViewMatrix[8] = view[8];
    mViewMatrix[9] = view[9];
    mViewMatrix[10] = view[10];
    mViewMatrix[11] = view[11];
    mViewMatrix[12] = view[12];
    mViewMatrix[13] = view[13];
    mViewMatrix[14] = view[14];
    mViewMatrix[15] = view[15];

    mProjectionMatrix[0] = projection[0];
    mProjectionMatrix[1] = projection[1];
    mProjectionMatrix[2] = projection[2];
    mProjectionMatrix[3] = projection[3];
    mProjectionMatrix[4] = projection[4];
    mProjectionMatrix[5] = projection[5];
    mProjectionMatrix[6] = projection[6];
    mProjectionMatrix[7] = projection[7];
    mProjectionMatrix[8] = projection[8];
    mProjectionMatrix[9] = projection[9];
    mProjectionMatrix[10] = projection[10];
    mProjectionMatrix[11] = projection[11];
    mProjectionMatrix[12] = projection[12];
    mProjectionMatrix[13] = projection[13];
    mProjectionMatrix[14] = projection[14];
    mProjectionMatrix[15] = projection[15];
}

void XFilter::submit() {
    if (!isValid()) {
        return;
    }

    init();

    if (!bgfx::isValid(mProgram)) {
        mProgram = loadProgram(mVertexShaderPath.data(), mFragmentShaderPath.data());
        mTexture = bgfx::createUniform("s_texColor0", bgfx::UniformType::Sampler);
        // Create vertex stream declaration.
        PosTexVertex::init();

        // Create static vertex buffer.
        mVertexBuffer = bgfx::createVertexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_vertices, sizeof(s_vertices) )
                , PosTexVertex::ms_decl);

        // Create static index buffer for triangle list rendering.
        mIndexBuffer = bgfx::createIndexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_triList, sizeof(s_triList) ));
    }
    
    if (!bgfx::isValid(mProgram)) {
        LOGE("[XFilter::submit] load program failed. vsPath=%s, fsPath=%s", mVertexShaderPath.data(), mFragmentShaderPath.data());
        return;
    }

    int renderIndex = XImage::nextRenderIndex();
    bgfx::setViewRect(renderIndex, mRect.x, mRect.y, mRect.width, mRect.height);
    bgfx::setViewClear(renderIndex, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00000000, 1.0f, 0);
    bgfx::touch(renderIndex);
    if (mOutputFrameBuffer != nullptr) {
        bgfx::setViewFrameBuffer(renderIndex, mOutputFrameBuffer->get());
    } else {
        bgfx::setViewFrameBuffer(renderIndex, BGFX_INVALID_HANDLE);
    }
    bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
    bgfx::setVertexBuffer(0, mVertexBuffer);
    bgfx::setIndexBuffer(mIndexBuffer);
    bgfx::setTexture(0, mTexture, mFirstInputFrameBuffer->getTexture());
    if (mViewMatrix != nullptr && mProjectionMatrix != nullptr) {
        bgfx::setViewTransform(renderIndex, mViewMatrix, mProjectionMatrix);
    }
    updateParams();
    bgfx::submit(renderIndex, mProgram);
    XOutput::submit();
}

void XFilter::setVec4(std::string paramName, glm::vec4 &value) {
    mParams[paramName] = value;
}

void XFilter::updateParams() {
    for (std::pair<std::string, glm::vec4> param : mParams) {
        bgfx::UniformHandle handle;
        auto iter = mParamHandles.find(param.first);
        if (iter == mParamHandles.end()) {
            handle = bgfx::createUniform(param.first.data(), bgfx::UniformType::Vec4);
            mParamHandles.insert(std::make_pair(param.first, handle));
        } else {
            handle = iter->second;
        }
        if (!bgfx::isValid(handle)) {
            LOGE("XFilter::setVec4 uniform handle is invalid.");
        }
        float *paramValue = new float[4]{param.second[0], param.second[1], param.second[2], param.second[3]};
        bgfx::setUniform(handle, paramValue);
        SAFE_DELETE_ARRAY(paramValue);
    }
}

bool XFilter::isValid() {
    if (mRect.width <= 0 || mRect.height <= 0) {
        LOGE("[XFilter::submit] view size args is invalid.");
        return false;
    }
    if (mFirstInputFrameBuffer == nullptr || !bgfx::isValid(mFirstInputFrameBuffer->getTexture())) {
        LOGE("[XFilter::submit] input frame is invalid.");
        return false;
    }
    return true;
}
NS_X_IMAGE_END
