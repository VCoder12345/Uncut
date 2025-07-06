#include "LibModel.h"
void LibModel::addItem(LibItemData *data) {
  itemDataList.push_back(data);

  emit itemAdded(data);
}

int LibModel::getSelectedItem() { return selectedItem; }

void LibModel::setSelectedItem(int selected) {
  int oldSelected = this->selectedItem;
  if (oldSelected >= 0)
    itemDataList[oldSelected]->selected = false;

  itemDataList[selected]->selected = true;
  this->selectedItem = selected;

  emit itemSelected(oldSelected, selected, itemDataList[selected]);
}

LibModel::~LibModel() {
  for (LibItemData *data : itemDataList) {
    delete data;
  }
}

