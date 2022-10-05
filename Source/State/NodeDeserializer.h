#pragma once

#include "rapidjson/document.h"

#include "NodeVisitor.h"

namespace NodeDeserializer
{
Ptr<GuiOperator> createOperator(const rapidjson::Value& value);
Ptr<GuiSequence> createSequence(const rapidjson::Value& value);
Ptr<GuiTransform> createTransform(const rapidjson::Value& value);

void assignCommon(const rapidjson::Value& value, Ptr<GuiNode> node);
void assignSequence(const rapidjson::Value& value, Ptr<GuiSequence> sequence);
} // namespace NodeDeserializer
