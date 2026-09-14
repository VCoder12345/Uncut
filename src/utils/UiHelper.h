#pragma once
#include <qfontmetrics.h>
#include <QString>

class UiHelper
{
public:
	static QString shortenTextTo(QString txt, int width, const QFont& font)
	{
		QFontMetrics metrics(font);
		int txtWidth = metrics.horizontalAdvance(txt);
		if (txtWidth > width) {
			int sz = txt.size();
			for (int i = sz - 1; i >= 0; --i) {
				QString nameSlice = txt.left(i) + "...";
				txtWidth = metrics.horizontalAdvance(nameSlice);

				if (txtWidth <= width) {
					txt = nameSlice;
					break;
				}
			}
		}

		return txt;
	}
};

