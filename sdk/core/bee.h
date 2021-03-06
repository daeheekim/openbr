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

#ifndef __BEE_H
#define __BEE_H

#include <QList>
#include <QPair>
#include <QHash>
#include <QString>
#include <QStringList>
#include <opencv2/core/core.hpp>
#include <openbr_plugin.h>

/*** Functions for parsing BEE style data structures. ***/
namespace BEE
{
    const uchar Match(0xff);
    const uchar NonMatch(0x7f);
    const uchar DontCare(0x00);
    typedef float Simmat_t;
    typedef uchar Mask_t;

    // Sigset IO
    br::FileList readSigset(QString sigset, bool ignoreMetadata = false);
    void writeSigset(const QString &sigset, const br::FileList &metadataList);

    // Matrix IO
    cv::Mat readSimmat(const br::File &simmat);
    cv::Mat readMask(const br::File &mask);
    void writeSimmat(const cv::Mat &m, const QString &simmat, const QString &targetSigset = "Unknown_Target", const QString &querySigset = "Unknown_Query");
    void writeMask(const cv::Mat &m, const QString &mask, const QString &targetSigset = "Unknown_Target", const QString &querySigset = "Unknown_Query");

    // Write BEE files
    void makeMask(const QString &targetInput, const QString &queryInput, const QString &mask);
    void combineMasks(const QStringList &inputMasks, const QString &outputMask, const QString &method);
}

#endif // __BEE_H
