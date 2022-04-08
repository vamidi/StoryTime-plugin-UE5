#pragma once

#include "IDetailCustomization.h"
#include "Components/Base/DataAssets/TableDA.h"

#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Slate/Public/Widgets/Views/SListView.h"

class UTableDA;
class SComboButton;

struct FTableListData
{
	uint32 ID;
	FString Name = "Untitled";
};

class FTableSelector : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
private:
	FText GetCurrentItemLabel() const;
	TSharedRef<SWidget> GetListContent();
	void HandleMenuOpen();

	/** Return the representation of the the row names to display */
	TSharedRef<ITableRow> HandleRowNameComboBoxGenerateWidget(TSharedPtr<FString> InItem, const TSharedRef<STableViewBase>& OwnerTable) const;

	/** Update the root data on a change of selection */
	void OnSelectionChanged(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo);

	/** Called by Slate when the filter box changes text. */
	void OnFilterTextChanged(const FText& InFilterText);

	// TArray<TSharedPtr<FTableListData>> TableListData;
	TArray<TSharedPtr<FString>> TableListNames;

	/** The comboButton objects */
	TSharedPtr<SComboButton> RowNameComboButton;
	TSharedPtr<class SSearchBox> SearchBox;
	TSharedPtr<SListView<TSharedPtr<FString>>> RowNameComboListView;

	TMap<uint32, FString> PopulatedList;

	TSharedPtr<IPropertyHandle> IDHandle;
	TWeakObjectPtr<UTableDA> TableAsset;

	TSharedPtr<FString> CurrentItem;

	uint32 CurrentID = 0;
	bool IsJsonObj = false;
};
