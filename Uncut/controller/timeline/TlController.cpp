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
	connect(tlView, &TlView::deleteKeyPressed, this, &TlController::onDeleteKeyPressed);
}

//remove unused tracks that are above the last used one and keeps always at least one track
void TlController::removeUnusedTracks()
{
	for (int i = model->tracks.size() - 1; i >= 1; --i)
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


void TlController::onClipItemMoved(ClipData* data, double newPos, int newTrack, bool clipExists)
{
	if (newTrack >= model->tracks.size())
	{
		model->addTrack();
	}

	if (clipExists)
	{
		model->moveClip(data, newPos, newTrack);
		removeUnusedTracks();
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

void TlController::onDeleteKeyPressed()
{
	if (model->slcClip)
		model->removeSelection();

	removeUnusedTracks();
}






