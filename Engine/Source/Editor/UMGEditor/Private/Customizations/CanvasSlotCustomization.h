// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

class FCanvasSlotCustomization : public IPropertyTypeCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<class IPropertyTypeCustomization> MakeInstance(UBlueprint* Blueprint)
	{
		return MakeShareable(new FCanvasSlotCustomization(Blueprint));
	}

	FCanvasSlotCustomization(UBlueprint* InBlueprint)
		: Blueprint(Cast<UWidgetBlueprint>(InBlueprint))
	{
	}
	
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	
private:

	void CustomizeLayoutData(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);
	void CustomizeAnchors(TSharedPtr<IPropertyHandle> PropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);
	void CustomizeOffsets(TSharedPtr<IPropertyHandle> PropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);
	
	void FillOutChildren(TSharedRef<IPropertyHandle> PropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

	void CreateEditorWithDynamicLabel(IDetailPropertyRow& PropertyRow, TAttribute<FText> TextAttribute);

	static FText GetOffsetLabel(TSharedPtr<IPropertyHandle> AnchorStructureHandle, EOrientation Orientation, FText NonStretchingLabel, FText StretchingLabel);
	
	FReply OnAnchorClicked(TSharedPtr<IPropertyHandle> AnchorsHandle, FAnchors Anchors);

private:
	UWidgetBlueprint* Blueprint;
	
};
