#pragma once
#include "pch.h"
#include "Widget.h"

struct HaarCascadeProperties
{
    double ScaleFactor = 1.05;
    int MinNeighbors = 10;
    cv::Size FaceMinSize = cv::Size(50, 50);
    cv::Size EyeMinSize = cv::Size(15, 15);
    cv::Scalar FaceColor = cv::Scalar(50, 0, 200);
    cv::Scalar EyeColor = cv::Scalar(200, 0, 50);
    int Thickness = 2;
};

struct DNNProperties
{
    float ConfidenceThreshold = 0.4f;
    cv::Size InputImageSize = cv::Size(300, 300);
};

class PropertyWidget :
    public Widget
{
public:
    PropertyWidget(const Vec2& _Size);
    virtual ~PropertyWidget();

public:
    virtual void Initialize() override;
    virtual void Update() override;

public:
    const HaarCascadeProperties& GetHaarCascadeProperties() { return MHaarCascadeProperties; }

private:
    void ProcessHaarCascade();
    void ProcessDNN();

private:
    int MAlgorithmSelector;

    /* Haar Cascade 알고리즘 */
    HaarCascadeProperties MHaarCascadeProperties;
    std::string MFaceXmlPath;
    std::string MEyeXmlPath;

    bool BProcessed = false;

    /* DNN 알고리즘*/
    DNNProperties MDNNProperties;
    const std::string CLASSES[21] = { "background", "aeroplane", "bicycle", "bird", "boat",
    "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
    "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
    "sofa", "train", "tvmonitor" };

    std::string MCaffeModelPath;
    std::string MPrototxtPath;

};

