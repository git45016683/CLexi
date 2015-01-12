#include "stdafx.h"
#include "LxCommand.h"

LxInsertCmd::LxInsertCmd(size_t ins_pos, size_t src_font, COLORREF src_color)
	: ins_pos(ins_pos), src_font(src_font), src_color(src_color)
{
}
void LxInsertCmd::Excute()
{
	//1.���޸������ĵ�
	//2.�����޸ĵ������ĵ���λ�õ�����Ӧ���Ű溯��
	//3.���μ���ʣ���ĵ��ĶΡ�ҳ��ϵ
}
void LxInsertCmd::Undo()
{

}

LxDeleteCmd::~LxDeleteCmd()
{

}
void LxDeleteCmd::Excute()
{

}
void LxDeleteCmd::Undo()
{

}

void LxModifyViewCmd::Excute()
{

}

LxMergeCmd::LxMergeCmd(ComposeParagraph* paragraph1, ComposeParagraph* paragraph2)
	: paragraph1_(paragraph1), paragraph2_(paragraph2)
{
}
LxMergeCmd::~LxMergeCmd()
{

}
void LxMergeCmd::Excute()
{

}
void LxMergeCmd::Undo()
{

}

LxSplitCmd::LxSplitCmd(ComposeParagraph* paragraph)
	: paragraph(paragraph)
{
}
LxSplitCmd::~LxSplitCmd()
{

}
void LxSplitCmd::Excute()
{

}
void LxSplitCmd::Undo()
{

}

LxCommand::~LxCommand()
{
	for (LxCommandBase* it : command)
		delete it;
}
void LxCommand::add_child_cmd(LxCommandBase* child_cmd)
{
	command.push_back(child_cmd);
}
void LxCommand::Excute()
{
	for (LxCommandBase* it : command)
	{
		it->Excute();
	}
}
bool LxCommand::CanUndo()
{
	for (LxCommandBase* it : command)
	{
		if (!it->CanUndo()) return false;
	}
	return true;
}
void LxCommand::Undo()
{
	if (CanUndo())
	{
		auto rit = command.rbegin();
		auto rite = command.rend();
		for (; rit != rite; rit++)
		{
			(*(rit))->Undo();
		}
	}
}

LxCommandMgr::LxCommandMgr()
{
	LxCommand* empty_cmd = new LxCommand();
	empty_cmd->add_child_cmd(new LxEmptyCmd());
	command_list.push_back(empty_cmd);
	curr_ = command_list.begin();
}
LxCommandMgr::~LxCommandMgr()
{
	for (LxCommand* it : command_list)
		delete it;
}
void LxCommandMgr::insert_cmd(LxCommand* lx_cmd)
{
	if (!lx_cmd->CanUndo())
	{
		delete lx_cmd;
		return;
	}
	if (curr_ == command_list.begin())
	{
		command_list.push_back(lx_cmd);
		curr_++;
		return;
	}
	auto it = curr_;
	it++;
	for (; it != command_list.end();)
	{
		delete *it;
		it = command_list.erase(it);
	}
	command_list.push_back(lx_cmd);
	curr_++;
}
LxCommand* LxCommandMgr::get_redo_cmd()
{
	if (curr_ == --command_list.end()) return nullptr;
	curr_++;
	return *curr_;
}
LxCommand* LxCommandMgr::get_undo_cmd()
{
	if (curr_ == command_list.begin()) return nullptr;
	LxCommand* cmd = *curr_;
	curr_--;
	return cmd;
}