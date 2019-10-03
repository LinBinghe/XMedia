//
// Created by 林炳河 on 2019/9/21.
//

#ifndef GPUIMAGE_X_XFILTEREFFECT_HPP
#define GPUIMAGE_X_XFILTEREFFECT_HPP

#include "XEffect.hpp"
#include "XFilter.hpp"
#include <unordered_map>
#include <glm/vec4.hpp>

NS_X_IMAGE_BEGIN
struct XFilterParam {
    std::string name;
    glm::vec4 valueMin;
    glm::vec4 valueMax;
    glm::vec4 valueDefault;
    glm::vec4 value;
    int valueNum;
};

/**
 * @brief 滤镜效果基类，负责控制滤镜通用逻辑
 *
 * 所有滤镜类都将具备以下几个内容：
 * 1.滤镜参数名
 * 2.滤镜参数的最大、最小以及默认值（特殊情况会在类注释上说明）
 * 3.滤镜参数的设置值
 * 通过以上内容来提供给使用者更直观的使用说明和参数设置
 */
class XFilterEffect: public XEffect {
public:
    XFilterEffect();

    virtual ~XFilterEffect();

    XInputOutput *get() override;

    /**
     * @brief 获取指定的参数数据
     * @param name 参数名
     * @return 参数数据
     */
    XFilterParam getParam(std::string name);

    /**
     * @brief 更新指定参数值
     * @param name 参数名
     * @param value 参数值
     */
    void updateValue(std::string name, glm::vec4 value);

    /**
     * @brief 更新参数值
     * @param params 参数列表
     */
    void updateValue(std::unordered_map<std::string, glm::vec4> params);

    /**
     * @brief 获取参数集合
     * @return 参数集合
     */
    std::unordered_map<std::string, XFilterParam> getParams();

protected:
    /**
     * @brief 初始化接口，子类可继承该接口实现自身的初始化
     */
    virtual void init();

    /**
     * @brief 添加参数信息到参数列表中
     * @param name 参数名
     * @param valueMin 最小值
     * @param valueMax 最大值
     * @param valueDefault 默认值
     * @param value 当前值
     * @attention 参数如果已存在那么会直接替换掉已有参数
     */
    void addParam(std::string name, float valueMin, float valueMax, float valueDefault, float value);

    /**
     * @brief 添加参数信息到参数列表中
     * @param name
     * @param valueMin 最小值
     * @param valueMax 最大值
     * @param valueDefault 默认值
     * @param value 当前值
     * @param valueNum 四位向量中有效值的位数
     * @attention 参数如果已存在那么会直接替换掉已有参数
     */
    void addParam(std::string name, glm::vec4 valueMin, glm::vec4 valueMax, glm::vec4 valueDefault, glm::vec4 value, int valueNum);

    /**
     * @brief 添加参数信息到参数列表中
     * @param param 参数信息
     * @attention 参数如果已存在那么会直接替换掉已有参数
     */
    void addParam(XFilterParam param);

    /**
     * @brief 设置顶点着色器名
     * @param name 顶点着色器名
     */
    void setVertexShaderName(std::string name);

    /**
     * @brief 设置片段着色器名
     * @param name 片段着色器名
     */
    void setFragmentShaderName(std::string name);

protected:
    XFilter *mFilter; /// 滤镜处理类
    std::string mVertexShaderName; /// 顶点着色器名
    std::string mFragmentShaderName; /// 片段着色器名

private:
    std::unordered_map<std::string, XFilterParam> mParams; /// 参数集合
};

/**
 * @brief 饱和度调节滤镜
 */
class XSaturation : public XFilterEffect {
public:
    XSaturation() : XFilterEffect() {
        setFragmentShaderName("fs_saturation");
        addParam("saturation", 0.0f, 2.0f, 1.0f, 1.0f);
    }

    std::string getName() override {
        return "Saturation";
    }
};

/**
 * @brief 对比度调节滤镜
 */
class XContrast : public XFilterEffect {
public:
    XContrast() : XFilterEffect() {
        setFragmentShaderName("fs_contrast");
        addParam("contrast", 0.0f, 4.0f, 1.0f, 1.0f);
    }

    std::string getName() override {
        return "Contrast";
    }
};

/**
 * @brief 亮度调节滤镜
 */
class XBrightness : public XFilterEffect {
public:
    XBrightness() : XFilterEffect() {
        setFragmentShaderName("fs_brightness");
        addParam("brightness", -1.0f, 1.0f, 0.0f, 0.0f);
    }

    std::string getName() override {
        return "Brightness";
    }
};

/**
 * @brief 曝光度调节滤镜
 */
class XExposure : public XFilterEffect {
public:
    XExposure() : XFilterEffect() {
        setFragmentShaderName("fs_exposure");
        addParam("exposure", -10.0f, 10.0f, 0.0f, 0.0f);
    }

    std::string getName() override {
        return "Exposure";
    }
};

/**
 * @brief HUE调节滤镜
 */
class XHUE : public XFilterEffect {
public:
    XHUE() : XFilterEffect() {
        setFragmentShaderName("fs_hue");
        addParam("hueAdjust", 0.0f, 2 * M_PI, 0.0f, 0.0f);
    }

    std::string getName() override {
        return "HUE";
    }
};

/**
 * @brief RGB 通道调节滤镜
 */
class XRGB : public XFilterEffect {
public:
    XRGB() : XFilterEffect() {
        setFragmentShaderName("fs_rgb");
        addParam("redAdjustment", 0.0f, 10.0f, 1.0f, 1.0f);
        addParam("greenAdjustment", 0.0f, 10.0f, 1.0f, 1.0f);
        addParam("blueAdjustment", 0.0f, 10.0f, 1.0f, 1.0f);
    }

    std::string getName() override {
        return "RGB";
    }
};

/**
 * @brief 白平衡调节滤镜
 */
class XWhiteBalance : public XFilterEffect {
public:
    XWhiteBalance() : XFilterEffect() {
        setFragmentShaderName("fs_white_balance");
        addParam("temperature", -50.0f, 50.0f, 0.0f, 0.0f);
        addParam("tint", -10.0f, 10.0f, 0.0f, 0.0f);
    }

    std::string getName() override {
        return "WhiteBalance";
    }
};

/**
 * @brief 待补充
 */
class XLevels : public XFilterEffect {
public:
    XLevels() : XFilterEffect() {
        setFragmentShaderName("fs_levels");
        glm::vec4 valueZero = {0.0f, 0.0f, 0.0f, 0.0f};
        glm::vec4 valueOne = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec4 valueHundred = {100.0f, 100.0f, 100.0f, 100.0f};
        addParam("levelMinimum", valueZero, valueOne, valueZero, valueZero, 3);
        addParam("levelMaximum", valueZero, valueOne, valueOne, valueOne, 3);
        addParam("levelMiddle", valueZero, valueHundred, valueOne, valueOne, 3);
        addParam("minOutput", valueZero, valueOne, valueZero, valueZero, 3);
        addParam("maxOutput", valueZero, valueOne, valueOne, valueOne, 3);
    }

    std::string getName() override {
        return "Levels";
    }
};

/**
 * @brief 待补充
 */
class XMonochrome : public XFilterEffect {
public:
    XMonochrome() : XFilterEffect() {
        setFragmentShaderName("fs_monochrome");
        addParam("intensity", 0.0f, 1.0f, 0.0f, 0.0f);
        glm::vec4 valueMin = {0.0f, 0.0f, 0.0f, 0.0f};
        glm::vec4 valueMax = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec4 valueDefault = {0.5f, 0.5f, 0.5f, 0.5f};
        glm::vec4 value = valueDefault;
        addParam("filterColor", valueMin, valueMax, valueDefault, value, 3);
    }

    std::string getName() override {
        return "Monochrome";
    }
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFILTEREFFECT_HPP