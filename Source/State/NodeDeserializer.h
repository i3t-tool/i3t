#pragma once

#include <optional>

#include "rapidjson/document.h"

#include "NodeVisitor.h"
#include "Stateful.h"

namespace NodeDeserializer
{
/// @pre DIWNE and WorkspaceWindow are initialized.
std::vector<Ptr<GuiNode>> createFrom(const Memento& memento);

std::optional<Ptr<GuiOperator>> createOperator(const rapidjson::Value& value);
Ptr<GuiSequence> createSequence(const rapidjson::Value& value);
std::optional<Ptr<GuiTransform>> createTransform(const rapidjson::Value& value);

void assignCommon(const rapidjson::Value& value, Ptr<GuiNode> node);
void assignSequence(const rapidjson::Value& value, Ptr<GuiSequence> sequence);
} // namespace NodeDeserializer
