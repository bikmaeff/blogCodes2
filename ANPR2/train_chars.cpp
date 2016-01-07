#include "train_chars.hpp"

#include <ocv_libs/file/utility.hpp>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

train_chars::train_chars(std::string chars_folder,
                         std::string result_folder) :
    bbps_({30, 15}, {{5,5}, {5,10}, {10,5}, {10,10}}),
    chars_folder_(std::move(chars_folder)),
    result_folder_(std::move(result_folder))
{
    using namespace boost::filesystem;

    if(!boost::filesystem::exists(path(result_folder_))){
        create_directory(path(result_folder_));
    }

    size_t index = 0;
    for(char i = '0'; i <= '9'; ++i){
        labels_to_int_.insert({std::string(1,i), index++});
    }
    for(char i = 'a'; i <= 'z'; ++i){
        labels_to_int_.insert({std::string(1,i), index++});
    }
    for(char i = 'A'; i <= 'Z'; ++i){
        labels_to_int_.insert({std::string(1,i),
                               labels_to_int_[std::string(1,std::tolower(i))]});
    }
}

void train_chars::extract_features()
{
    auto const folders =
            ocv::file::get_directory_folders(chars_folder_);
    for(size_t i = 0; i != folders.size(); ++i){
        auto const folder = chars_folder_ + "/" + folders[i];
        std::cout<<folder<<std::endl;
        int const labels = labels_to_int_[folders[i]];
        auto const files =
                ocv::file::get_directory_files(folder);
        for(size_t j = 0; j != files.size(); ++j){
            std::cout<<folder + "/" + files[j]<<std::endl;
            auto img = cv::imread(folder + "/" + files[j]);
            if(!img.empty()){
                std::cout<<"can read file"<<std::endl;
                auto const &feature = bbps_.describe(img);
                features_.push_back(cv::Mat(feature, true));
                labels_.emplace_back(labels);
            }
        }
    }
}

void train_chars::train_svm()
{
    using namespace cv::ml;

    auto svm = cv::ml::SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER,
                                          100, 1e-6));

    auto train_data = TrainData::create(features_.reshape(1, int(labels_.size())),
                                        ROW_SAMPLE,
                                        labels_);
    svm->trainAuto(train_data);
    std::cout<<"c : "<<svm->getC()<<"\n";
    std::cout<<"coef0 : "<<svm->getCoef0()<<"\n";
    std::cout<<"degree : "<<svm->getDegree()<<"\n";
    std::cout<<"gamma : "<<svm->getGamma()<<"\n";

    svm->write(cv::FileStorage("chars_classifier.xml",
                               cv::FileStorage::WRITE));

    ml_ = svm;
}

cv::Ptr<cv::ml::StatModel> train_chars::train()
{
    extract_features();
    //train_svm();

    return ml_;
}