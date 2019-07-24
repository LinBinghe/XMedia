//
// Created by 林炳河 on 2019/7/21.
//

#ifndef GPUIMAGE_X_XIMAGEUTILS_HPP
#define GPUIMAGE_X_XIMAGEUTILS_HPP

#include "bgfx/bgfx.h"
#include "bimg/bimg.h"
#include "bimg/decode.h"
#include <bx/allocator.h>
#include "bx/file.h"
#include "XMacros.hpp"

NS_X_IMAGE_BEGIN
/**
 * @brief XImage's utils to make develop easier.
 */
class XImageUtils {
public:
    /**
     * @brief load texture from absolute image path
     * @param[in] path frame path
     * @param[in] flags Texture creation (see `BGFX_TEXTURE_*`.), and sampler (see `BGFX_SAMPLER_*`)
     *              Default texture sampling mode is linear, and wrap mode is repeat.
     *              - `BGFX_SAMPLER_[U/V/W]_[MIRROR/CLAMP]` - Mirror or clamp to edge wrap mode.
     *              - `BGFX_SAMPLER_[MIN/MAG/MIP]_[POINT/ANISOTROPIC]` - Point or anisotropic sampling.
     * @param[in] skip don't know what it means yet
     * @param[in] info resulting texture info structure. See: `bgfx::TextureInfo`.
     * @param[in] orientation texture's orientation. See: `bimg::Orientation`.
     * @return handle of texture
     */
    static bgfx::TextureHandle
    loadTexture(const char *path, uint64_t flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t skip = 0,
                bgfx::TextureInfo *info = NULL, bimg::Orientation::Enum *orientation = NULL);

    /**
     * @brief load program from vertex and fragment shader absolute path
     * @param[in] vsPath absolute path of vertex shader
     * @param[in] fsPath absolute path of fragment shader
     * @return handle of program
     */
    static bgfx::ProgramHandle loadProgram( const char* vsPath, const char* fsPath );

    /**
     * @brief wrap float value to vec4 needed form
     * @param[in] value float value
     * @return float array, length:4
     */
    static float* wrapFloatToVec4(float value);

    /**
     * @brief wrap three float value to vec4 needed form
     * @param[in] x first float value
     * @param[in] y second float value
     * @param[in] z third float value
     * @return float array, length:4
     */
    static float* wrapVec3ToVec4(float x, float y, float z);

private:
    /**
     * @brief load texture from absolute file path
     * @param reader file reader
     * @param filePath frame path
     * @param[in] flags Texture creation (see `BGFX_TEXTURE_*`.), and sampler (see `BGFX_SAMPLER_*`)
     *              Default texture sampling mode is linear, and wrap mode is repeat.
     *              - `BGFX_SAMPLER_[U/V/W]_[MIRROR/CLAMP]` - Mirror or clamp to edge wrap mode.
     *              - `BGFX_SAMPLER_[MIN/MAG/MIP]_[POINT/ANISOTROPIC]` - Point or anisotropic sampling.
     * @param[in] skip don't know what it means yet
     * @param[in] info resulting texture info structure. See: `bgfx::TextureInfo`.
     * @param[in] orientation texture's orientation. See: `bimg::Orientation`.
     * @return handle of texture
     */
    static bgfx::TextureHandle
    loadTexture(bx::FileReaderI *reader, const char *filePath, uint64_t flags, uint8_t skip,
                bgfx::TextureInfo *_info, bimg::Orientation::Enum *orientation);

    /**
     * @brief load program from vertex and fragment shader absolute path
     * @param reader file reader
     * @param[in] vsPath absolute path of vertex shader
     * @param[in] fsPath absolute path of fragment shader
     * @return handle of program
     */
    static bgfx::ProgramHandle loadProgram(bx::FileReaderI* reader, const char* vsPath, const char* fsPath);

    /**
     * @brief load shader from absolute file path
     * @param[in] reader file reader
     * @param[in] path file path
     * @return handle of shader
     */
    static bgfx::ShaderHandle loadShader(bx::FileReaderI* reader, const char* path);

    /**
     * @brief load data of file to memory
     * @param[in] reader file reader
     * @param[in] filePath file path
     * @return data stored in `bgfx::Memory`
     */
    static const bgfx::Memory* loadMem(bx::FileReaderI* reader, const char* filePath);

    /**
     * @brief get reader to read file
     * @return reader to read file
     */
    static bx::FileReaderI* getFileReader();
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEUTILS_HPP