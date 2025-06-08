#include "TlController.h"

TlController::TlController(Uncut& window, TlModel* model) : model(model), tlView(window.getTlView())
{
	tlView->setModel(model);

	model->addTrack();
	model->addTrack();
	model->addTrack();

	model->addClip(new ClipData(nullptr, "", 0, 5.0, 0.0), 1);
	model->addClip(new ClipData(nullptr, "", 0, 2.0, 0.0), 2);
	model->addClip(new ClipData(nullptr, "", 0, 1.0, 0.0), 0);
	model->addClip(new ClipData(nullptr, "", 0, 4.0, 0.0), 2);

	connect(tlView, &TlView::clipItemMoved, this, &TlController::onClipItemMoved);
	connect(tlView, &TlView::requestClipSelect, this, &TlController::onClipSelectRequested);
}




void TlController::onClipItemMoved(ClipData* data, double newPos, int newTrack, bool clipExists)
{
	if (newTrack >= model->tracks.size())
	{
		model->addTrack();
	}

	if (clipExists)
	{
		model->moveClip(data, newPos, newTrack);
		for (int i = model->tracks.size() - 1; i >= 0; --i)
		{
			if (model->tracks[i]->clips.size() == 0)
			{
				model->removeLastTrack();
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		data->pos = newPos;
		model->addClip(data, newTrack);
	}

}



void TlController::onClipSelectRequested(ClipData* data)
{
	model->select(data);
}






