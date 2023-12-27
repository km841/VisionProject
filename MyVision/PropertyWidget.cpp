#include "PropertyWidget.h"
#include "RenderManager.h"
#include "Texture.h"
#include "opencv2/dnn.hpp"
using namespace cv::dnn;

PropertyWidget::PropertyWidget(const Vec2& _Size)
	: Widget(_Size)
    , MAlgorithmSelector(0)
{
}

PropertyWidget::~PropertyWidget()
{
}

void PropertyWidget::Initialize()
{
    
}

void PropertyWidget::Update()
{
    static std::string ButtonNames[] =
    {
        "Haar_Cascade",
        "DNN_Algorithm",
    };
	if (ImGui::Begin("Property", nullptr))
	{
		// 라디오 버튼에 따라 옵션이 달라지도록
        ImGui::SetWindowFontScale(2.f);

        for (int i = 0; i < 2; ++i)
        {
            // 라디오 버튼을 그림
            if (ImGui::RadioButton(ButtonNames[i].c_str(), MAlgorithmSelector == i))
            {
                // 라디오 버튼이 클릭되면 선택된 옵션을 업데이트
                MAlgorithmSelector = i;
            }
        }

        ImGui::Separator();
        ImGui::Separator();

        if (MAlgorithmSelector == 0)
        {
            if (ImGui::Button("Open Face Xml", Vec2(300.f, 50.f)))
                ImGuiFileDialog::Instance()->OpenDialog("Open Face Xml", "Open Face File", ".xml", "..\\Resources\\Xml\\");

            if (ImGuiFileDialog::Instance()->Display("Open Face Xml"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    fs::path szPath = ImGuiFileDialog::Instance()->GetFilePathName();
                    MFaceXmlPath = szPath.string();
                }
                ImGuiFileDialog::Instance()->Close();
            }

            ImGui::SameLine();
            ImGui::Text(MFaceXmlPath.c_str());

            if (ImGui::Button("Open Eye Xml", Vec2(300.f, 50.f)))
                ImGuiFileDialog::Instance()->OpenDialog("Open Eye Xml", "Open Eye File", ".xml", "..\\Resources\\Xml\\");

            if (ImGuiFileDialog::Instance()->Display("Open Eye Xml"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    fs::path szPath = ImGuiFileDialog::Instance()->GetFilePathName();
                    MEyeXmlPath = szPath.string();
                }
                ImGuiFileDialog::Instance()->Close();
            }

            ImGui::SameLine();
            ImGui::Text(MEyeXmlPath.c_str());

            ImGui::Separator();
            ImGui::Separator();

            ImGui::InputDouble("ScaleFactor", &MHaarCascadeProperties.ScaleFactor);
            ImGui::InputInt("MinNeighbors", &MHaarCascadeProperties.MinNeighbors);
            ImGui::InputInt("Thickness", &MHaarCascadeProperties.Thickness);

            ImGui::SetCursorPos({ 0.f,  ImGui::GetWindowSize().y - 100.f });
            if (ImGui::Button("Cascade Run", Vec2(200.f, 100.f)))
            {
                ProcessHaarCascade();
            }
        }
        else
        {
            if (ImGui::Button("Open Prototxt File", Vec2(300.f, 50.f)))
                ImGuiFileDialog::Instance()->OpenDialog("Open Prototxt File", "Open Face File", ".prototxt", "..\\Resources\\DNN\\");

            if (ImGuiFileDialog::Instance()->Display("Open Prototxt File"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    fs::path szPath = ImGuiFileDialog::Instance()->GetFilePathName();
                    MPrototxtPath = szPath.string();
                }
                ImGuiFileDialog::Instance()->Close();
            }

            ImGui::SameLine();
            ImGui::Text(MPrototxtPath.c_str());

            if (ImGui::Button("Open Caffe Model", Vec2(300.f, 50.f)))
                ImGuiFileDialog::Instance()->OpenDialog("Open Caffe Model", "Open Caffe Model", ".caffemodel", "..\\Resources\\DNN\\");

            if (ImGuiFileDialog::Instance()->Display("Open Caffe Model"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    fs::path szPath = ImGuiFileDialog::Instance()->GetFilePathName();
                    MCaffeModelPath = szPath.string();
                }
                ImGuiFileDialog::Instance()->Close();
            }

            ImGui::SameLine();
            ImGui::Text(MCaffeModelPath.c_str());

            ImGui::SliderFloat("Confidence Threshold", &MDNNProperties.ConfidenceThreshold, 0.0f, 1.0f);
            ImGui::SliderInt2("Input Image Size", &MDNNProperties.InputImageSize.width, 100, 1000);  // Adjust as needed

            ImGui::SetCursorPos({ 0.f,  ImGui::GetWindowSize().y - 100.f });
            if (ImGui::Button("DNN Run", Vec2(200.f, 100.f)))
            {
                ProcessDNN();
            }
        }
	}
	ImGui::End();
}

void PropertyWidget::ProcessHaarCascade()
{
    if (MFaceXmlPath.empty() || MEyeXmlPath.empty())
        return;

    if (RENDER->GetTexture()->IsProcessed())
        return;

    cv::Mat Img = cv::imread(ws2s(RENDER->GetTexture()->GetPath()));

    cv::Mat grayImg;
    cv::cvtColor(Img, grayImg, cv::COLOR_BGR2GRAY);		// 정확도를 높히기 위해 gray 이미지로 변경

    std::vector<cv::Rect> faceRect, eyeRect;
    cv::CascadeClassifier face, eye;

    face.load(MFaceXmlPath);	// 얼굴 탐지 cascade load
    if (!face.empty())
        face.detectMultiScale(grayImg, faceRect, MHaarCascadeProperties.ScaleFactor, MHaarCascadeProperties.MinNeighbors, 0, MHaarCascadeProperties.FaceMinSize);

    eye.load(MEyeXmlPath);		// 눈 탐지 cascade load
    if (!eye.empty())
        eye.detectMultiScale(grayImg, eyeRect, MHaarCascadeProperties.ScaleFactor, MHaarCascadeProperties.MinNeighbors / 2, 0, MHaarCascadeProperties.EyeMinSize);

    for (int i = 0; i < faceRect.size(); i++)
        rectangle(Img, cv::Rect(faceRect[i]), MHaarCascadeProperties.FaceColor, MHaarCascadeProperties.Thickness);	// 얼굴 영역 그리기

    for (int i = 0; i < eyeRect.size(); i++)
        rectangle(Img, cv::Rect(eyeRect[i]), MHaarCascadeProperties.EyeColor, MHaarCascadeProperties.Thickness);	// 눈 영역 그리기


    std::shared_ptr<Texture> PTexture = std::make_shared<Texture>(OBJECT_TYPE::TEXTURE);
    PTexture->CreateFromMatrix(Img);

    RENDER->SetTexture(PTexture);
    PTexture->SetProcessed();
}

void PropertyWidget::ProcessDNN()
{
    if (RENDER->GetTexture()->IsProcessed())
        return;

    if (MPrototxtPath.empty() || MCaffeModelPath.empty())
        return;

    cv::Mat Img = cv::imread(ws2s(RENDER->GetTexture()->GetPath()));

    cv::dnn::Net net = cv::dnn::readNetFromCaffe(MPrototxtPath, MCaffeModelPath);
    cv::Mat blob = cv::dnn::blobFromImage(Img, 0.007843, MDNNProperties.InputImageSize, cv::Scalar(127.5, 127.5, 127.5));
    net.setInput(blob);
    cv::Mat detection = net.forward();

    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);
        if (confidence > MDNNProperties.ConfidenceThreshold)
        {
            int class_id = (int)(detectionMat.at<float>(i, 1));
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * Img.cols);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * Img.rows);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * Img.cols);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * Img.rows);

            cv::rectangle(Img, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);

            cv::String label = cv::format("%s: %.2f", CLASSES[class_id].c_str(), confidence);
            int baseline;
            cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
            cv::rectangle(Img, cv::Point(x1, y1 - label_size.height), cv::Point(x1 + label_size.width, y1), cv::Scalar(0, 255, 0), -1);
            cv::putText(Img, label, cv::Point(x1, y1), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
        }
    }

    std::shared_ptr<Texture> PTexture = std::make_shared<Texture>(OBJECT_TYPE::TEXTURE);
    PTexture->CreateFromMatrix(Img);

    RENDER->SetTexture(PTexture);
    PTexture->SetProcessed();
}
