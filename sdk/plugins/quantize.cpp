/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2012 The MITRE Corporation                                      *
 *                                                                           *
 * Licensed under the Apache License, Version 2.0 (the "License");           *
 * you may not use this file except in compliance with the License.          *
 * You may obtain a copy of the License at                                   *
 *                                                                           *
 *     http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                           *
 * Unless required by applicable law or agreed to in writing, software       *
 * distributed under the License is distributed on an "AS IS" BASIS,         *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 * See the License for the specific language governing permissions and       *
 * limitations under the License.                                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <openbr_plugin.h>

#include "core/opencvutils.h"

using namespace cv;

namespace br
{

/*!
 * \ingroup transforms
 * \brief Approximate floats as uchar.
 * \author Josh Klontz \cite jklontz
 */
class QuantizeTransform : public Transform
{
    Q_OBJECT
    Q_PROPERTY(float a READ get_a WRITE set_a RESET reset_a)
    Q_PROPERTY(float b READ get_b WRITE set_b RESET reset_b)
    BR_PROPERTY(float, a, 1)
    BR_PROPERTY(float, b, 0)

    void train(const TemplateList &data)
    {
        double minVal, maxVal;
        minMaxLoc(OpenCVUtils::toMat(data.data()), &minVal, &maxVal);
        a = 255.0/(maxVal-minVal);
        b = -a*minVal;
    }

    void project(const Template &src, Template &dst) const
    {
        src.m().convertTo(dst, CV_8U, a, b);
    }
};

BR_REGISTER(Transform, QuantizeTransform)

/*!
 * \ingroup transforms
 * \brief Approximate floats as signed bit.
 * \author Josh Klontz \cite jklontz
 */
class BinarizeTransform : public UntrainableTransform
{
    Q_OBJECT

    void project(const Template &src, Template &dst) const
    {
        const Mat &m = src;
        assert((m.cols % 8 == 0) && (m.type() == CV_32FC1));
        Mat n(m.rows, m.cols/8, CV_8UC1);
        for (int i=0; i<m.rows; i++)
            for (int j=0; j<m.cols-7; j+=8)
                n.at<uchar>(i,j) = ((m.at<float>(i,j+0) > 0) << 0) +
                                   ((m.at<float>(i,j+1) > 0) << 1) +
                                   ((m.at<float>(i,j+2) > 0) << 2) +
                                   ((m.at<float>(i,j+3) > 0) << 3) +
                                   ((m.at<float>(i,j+4) > 0) << 4) +
                                   ((m.at<float>(i,j+5) > 0) << 5) +
                                   ((m.at<float>(i,j+6) > 0) << 6) +
                                   ((m.at<float>(i,j+7) > 0) << 7);
        dst = n;
    }
};

BR_REGISTER(Transform, BinarizeTransform)

/*!
 * \ingroup transforms
 * \brief Compress two uchar into one uchar.
 * \author Josh Klontz \cite jklontz
 */
class PackTransform : public UntrainableTransform
{
    Q_OBJECT

    void project(const Template &src, Template &dst) const
    {
        const Mat &m = src;
        if ((m.cols % 2 != 0) || (m.type() != CV_8UC1))
            qFatal("Pack::project invalid template format.");

        Mat n(m.rows, m.cols/2, CV_8UC1);
        for (int i=0; i<m.rows; i++)
            for (int j=0; j<m.cols/2; j++)
                n.at<uchar>(i,j) = ((m.at<uchar>(i,2*j+0) >> 4) << 4) +
                                   ((m.at<uchar>(i,2*j+1) >> 4) << 0);
        dst = n;
    }
};

BR_REGISTER(Transform, PackTransform)

} // namespace br

#include "quantize.moc"
