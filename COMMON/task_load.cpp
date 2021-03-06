#pragma once

#include "task.h"

Date^ ReadDate(int version, BinaryReader^ br)
{
	Date^ date = gcnew Date();

	date->year = br->ReadInt32();
	date->month = br->ReadInt32();
	date->day = br->ReadInt32();
	date->hour = br->ReadInt32();
	date->minute = br->ReadInt32();
	date->weekday = br->ReadInt32();

	return date;
}

DateSpan^ ReadDateSpan(int version, BinaryReader^ br)
{
	DateSpan^ date_span = gcnew DateSpan();

	date_span->from = ReadDate(version, br);
	date_span->to = ReadDate(version, br);

	return date_span;
}

Item^ ReadItem(int version, BinaryReader^ br)
{
	Item^ item = gcnew Item();

	item->id = br->ReadInt32();
	item->unknown = br->ReadBoolean();
	item->amount = br->ReadInt32();
	item->probability = br->ReadSingle();

	if(version >= 89)
	{
		item->expiration = br->ReadInt32();
	}
	else
	{
		item->expiration = 0;
	}

	return item;
}

ItemGroup^ ReadItemGroup(int version, BinaryReader^ br)
{
	ItemGroup^ item_group = gcnew ItemGroup();

	item_group->unknown = br->ReadBoolean();
	item_group->items_count = br->ReadInt32();
	item_group->items = gcnew array<Item^>(item_group->items_count);
	for(int n=0; n<item_group->items->Length; n++)
	{
		item_group->items[n] = ReadItem(version, br);
	}

	return item_group;
}

TeamMembers^ ReadTeamMembers(int version, BinaryReader^ br)
{
	TeamMembers^ team_member_group = gcnew TeamMembers();

	team_member_group->level_min = br->ReadInt32();
	team_member_group->level_max = br->ReadInt32();
	team_member_group->unknown_1 = br->ReadInt32(); // race?
	team_member_group->unknown_2 = br->ReadInt32(); // occupation?
	team_member_group->unknown_3 = br->ReadInt32(); // gender?
	team_member_group->amount_min = br->ReadInt32();
	team_member_group->amount_max = br->ReadInt32();
	team_member_group->quest = br->ReadInt32();

	if(version >= 100)
	{
		team_member_group->force = br->ReadInt32();
	}
	else
	{
		team_member_group->force = 0;
	}

	return team_member_group;
}

MoraiPK^ ReadMoraiPK(int version, BinaryReader^ br)
{
	MoraiPK^ morai_pk = gcnew MoraiPK();

	if(version >= 103)
	{
		morai_pk->unknown_1 = br->ReadInt32();
		morai_pk->unknown_2 = br->ReadInt32();
		morai_pk->unknown_3 = br->ReadInt32();
		morai_pk->unknown_4 = br->ReadByte();
		morai_pk->probability = br->ReadSingle();
		morai_pk->class_mask = br->ReadInt32();
		morai_pk->level_min = br->ReadInt32();
		morai_pk->level_max = br->ReadInt32();
		morai_pk->unknown_5 = br->ReadInt32();
		morai_pk->type = br->ReadInt32();
	}
	else
	{
		morai_pk->unknown_1 = 1;
		morai_pk->unknown_2 = 0;
		morai_pk->unknown_3 = 1;
		morai_pk->unknown_4 = 0;
		morai_pk->probability = 1.0f;
		morai_pk->class_mask = 1023;
		morai_pk->level_min = 0;
		morai_pk->level_max = 150;
		morai_pk->unknown_5 = 0;
		morai_pk->type = 0;
	}

	return morai_pk;
}

Chase^ ReadChase(int version, BinaryReader^ br)
{
	Chase^ chase = gcnew Chase();

	chase->id_monster = br->ReadInt32();
	chase->amount_monster = br->ReadInt32();
	chase->id_drop = br->ReadInt32();
	chase->amount_drop = br->ReadInt32();
	chase->unknown_1 = br->ReadByte(); //is common?
	chase->probability = br->ReadSingle();
	chase->unknown_2 = br->ReadByte(); // killer level

	if(version >= 92)
	{
		chase->unknown_3 = br->ReadBytes(8); // int DPH, int DPS
	}
	else
	{
		chase->unknown_3 = gcnew array<unsigned char>(8);
	}

	return chase;
}

Location^ ReadLocation(int version, BinaryReader^ br)
{
	Location^ location = gcnew Location();

	location->map_id = br->ReadInt32();
	location->x = br->ReadSingle();
	location->altitude = br->ReadSingle();
	location->z = br->ReadSingle();

	return location;
}

Span^ ReadSpan(int version, BinaryReader^ br)
{
	Span^ location_span = gcnew Span();

	location_span->east = br->ReadSingle();
	location_span->bottom = br->ReadSingle();
	location_span->south = br->ReadSingle();
	location_span->west = br->ReadSingle();
	location_span->top = br->ReadSingle();
	location_span->north = br->ReadSingle();

	return location_span;
}

Reward^ ReadReward(int version, BinaryReader^ br)
{
	Reward^ reward = gcnew Reward();

	reward->coins = br->ReadInt32();
	reward->experience = br->ReadInt32();
	reward->new_quest = br->ReadInt32();
	reward->spirit = br->ReadInt32();
	reward->reputation = br->ReadInt32();
	reward->cultivation = br->ReadInt32();
	reward->new_waypoint = br->ReadInt32();
	reward->storage_slots = br->ReadInt32();

	if(version >= 89)
	{
		reward->cupboard_slots = br->ReadInt32();
		reward->wardrobe_slots = br->ReadInt32();
		reward->account_stash_slots = br->ReadInt32();
	}

	reward->inventory_slots = br->ReadInt32();
	reward->petbag_slots = br->ReadInt32();
	reward->vigor = br->ReadInt32();
	reward->teleport = ReadLocation(version, br);
	reward->ai_trigger = br->ReadInt32();
	reward->UNKNOWN_2a = br->ReadBytes(3);

	if(version > 55)
	{
		reward->UNKNOWN_2b = br->ReadBytes(5);
	}
	else
	{
		reward->UNKNOWN_2b = gcnew array<unsigned char>(5);
	}

	reward->item_groups_count = br->ReadInt32();
	reward->SEPERATOR = br->ReadBytes(4);

	reward->pq = gcnew PQ_Reward();
	if(version >= 89)
	{
		reward->pq->chase_count = br->ReadInt32();
		reward->pq->unknown_0 = br->ReadBytes(6);
		reward->pq->event_gold = br->ReadInt32();
		reward->pq->unknown_1 = br->ReadBytes(9);
		reward->pq->unknown_quest = br->ReadInt32();
		reward->pq->unknown_2 = br->ReadBytes(4);
		reward->pq->unknown_level = br->ReadInt32();
		reward->pq->contribution_random_min = br->ReadInt32();
		reward->pq->contribution_random_max = br->ReadInt32();
		reward->pq->contribution_required = br->ReadInt32();
		reward->pq->unknown_3 = br->ReadBytes(8);
		reward->pq->item_count = br->ReadInt32();
		reward->pq->unknown_4 = br->ReadBytes(4);
		reward->pq->special_count = br->ReadInt32();
		reward->pq->unknown_5 = br->ReadBytes(29);
		reward->pq->script_count = br->ReadInt32();
		reward->pq->unknown_6 = br->ReadBytes(8);
		reward->pq->message_count = br->ReadInt32();
		reward->pq->unknown_7 = br->ReadBytes(4);
	}
	else
	{
		reward->pq->chase_count = 0;
		reward->pq->unknown_0 = gcnew array<unsigned char>(6);
		reward->pq->event_gold = 0;
		reward->pq->unknown_1 = gcnew array<unsigned char>(9);
		reward->pq->unknown_quest = 0;
		reward->pq->unknown_2 = gcnew array<unsigned char>(4);
		reward->pq->unknown_level = 0;
		reward->pq->contribution_random_min = 0;
		reward->pq->contribution_random_max = 0;
		reward->pq->contribution_required = 0;
		reward->pq->unknown_3 = gcnew array<unsigned char>(8);
		reward->pq->item_count = 0;
		reward->pq->unknown_4 = gcnew array<unsigned char>(4);
		reward->pq->special_count = 0;
		reward->pq->unknown_5 = gcnew array<unsigned char>(29);
		reward->pq->script_count = 0;
		reward->pq->unknown_6 = gcnew array<unsigned char>(8);
		reward->pq->message_count = 0;
		reward->pq->unknown_7 = gcnew array<unsigned char>(4);
	}
	// initialize values to prevent NULL exception
	{
		reward->pq->chase_unknown_1 = false;
		reward->pq->chase_unknown_2 = 0;
		reward->pq->chase_unknown_3 = false;
		reward->pq->chases = gcnew array<PQ_Chase^>(0);

		reward->pq->item_unknown_1 = false;
		reward->pq->items = gcnew array<PQ_Item^>(0);

		reward->pq->specials = gcnew array<PQ_Special^>(0);

		reward->pq->scripts = gcnew array<array<unsigned char>^>(0);
		reward->pq->messages = gcnew array<array<unsigned char>^>(0);
	}

	if(version >= 100)
	{
		reward->influence = br->ReadInt32();
		reward->prestige = br->ReadInt32();
		reward->UNKNOWN_5 = br->ReadInt32();
		reward->UNKNOWN_6 = br->ReadInt32();
	}
	else
	{
		reward->influence = 0;
		reward->prestige = 0;
		reward->UNKNOWN_5 = 0;
		reward->UNKNOWN_6 = 0;
	}

	if(version >= 102)
	{
		reward->quest_slot_expansion = br->ReadInt32();
	}
	else
	{
		reward->quest_slot_expansion = 0;
	}

	reward->item_groups = gcnew array<ItemGroup^>(reward->item_groups_count);

	for(int i=0; i<reward->item_groups->Length; i++)
	{
		reward->item_groups[i] = ReadItemGroup(version, br);
	}

	if(version >= 89)
	{
		if(reward->pq->chase_count>0)
		{
			reward->pq->chase_unknown_1 = br->ReadBoolean();
			reward->pq->chase_unknown_2 = br->ReadInt32();
			reward->pq->chase_unknown_3 = br->ReadBoolean();
			reward->pq->chases = gcnew array<PQ_Chase^>(reward->pq->chase_count);
			for(int i=0; i<reward->pq->chase_count; i++)
			{
				reward->pq->chases[i] = gcnew PQ_Chase();
				reward->pq->chases[i]->id_monster = br->ReadInt32();
				reward->pq->chases[i]->amount_monster = br->ReadInt32();
				reward->pq->chases[i]->probability = br->ReadSingle();
				reward->pq->chases[i]->amount_unknown = br->ReadInt32();
			}
		}

		if(reward->pq->item_count>0)
		{
			reward->pq->item_unknown_1 = br->ReadBoolean();
			reward->pq->items = gcnew array<PQ_Item^>(reward->pq->item_count);
			for(int i=0; i<reward->pq->item_count; i++)
			{
				reward->pq->items[i] = gcnew PQ_Item;
				reward->pq->items[i]->ranking_from = br->ReadInt32();
				reward->pq->items[i]->ranking_to = br->ReadInt32();
				reward->pq->items[i]->unknown_3 = br->ReadBoolean();
				reward->pq->items[i]->id = br->ReadInt32();
				reward->pq->items[i]->amount = br->ReadInt32();
				reward->pq->items[i]->probability =  br->ReadSingle();
			}
		}

		if(reward->pq->special_count>0)
		{
			reward->pq->specials = gcnew array<PQ_Special^>(reward->pq->special_count);
			for(int i=0; i<reward->pq->special_count; i++)
			{
				reward->pq->specials[i] = gcnew PQ_Special();
				reward->pq->specials[i]->id_pq = br->ReadInt32();
				reward->pq->specials[i]->unknown_2 = br->ReadInt32();
				reward->pq->specials[i]->unknown_3 = br->ReadByte();
			}
		}

		if(reward->pq->script_count>0)
		{
			reward->pq->scripts = gcnew array<array<unsigned char>^>(reward->pq->script_count);
			for(int i=0; i<reward->pq->script_count; i++)
			{
				reward->pq->scripts[i] = br->ReadBytes(576);
			}
		}

		if(reward->pq->message_count>0)
		{
			reward->pq->messages = gcnew array<array<unsigned char>^>(reward->pq->message_count);
			for(int i=0; i<reward->pq->message_count; i++)
			{
				reward->pq->messages[i] = br->ReadBytes(128);
			}
		}
	}

	return reward;
}

void Task::Load(int version, BinaryReader^ br, int stream_position, TreeNodeCollection^ nodes)
{
	br->BaseStream->Position = stream_position;

// ################# GENERAL #############################

	id = br->ReadInt32();
	name = br->ReadBytes(60);
	author_mode = br->ReadBoolean();
	UNKNOWN_01 = br->ReadBytes(4); // author mode related pointer?
	type = br->ReadInt32();
	time_limit = br->ReadInt32();

	if(version >= 89)
	{
		UNKNOWN_02 = br->ReadBoolean(); // has offline fail
		has_date_fail = br->ReadBoolean();
		date_fail = ReadDate(version, br);
		UNKNOWN_03 = br->ReadBoolean(); // item off fail
	}
	else
	{
		UNKNOWN_02 = false;
		has_date_fail = false;
		date_fail = gcnew Date();
		date_fail->year = 0;
		date_fail->month = 0;
		date_fail->day = 0;
		date_fail->hour = 0;
		date_fail->minute = 0;
		date_fail->weekday = 0;
		UNKNOWN_03 = false;
	}

	has_date_spans = br->ReadBoolean();
	date_spans_count = br->ReadInt32();
	UNKNOWN_04 = br->ReadBytes(4); // time types?
	UNKNOWN_05 = br->ReadBytes(8);

	if(version >= 89)
	{
		UNKNOWN_06 = br->ReadBytes(12);
	}
	else
	{
		UNKNOWN_06 = gcnew array<unsigned char>(12);
	}

	UNKNOWN_07 = br->ReadBytes(8); // int replication_count + int period_limit ?
	UNKNOWN_08 = br->ReadInt32();

	if(version >= 89)
	{
		UNKNOWN_09 = br->ReadInt32();
	}
	else
	{
		UNKNOWN_09 = 0;
	}

	activate_first_subquest = br->ReadBoolean();
	activate_random_subquest = br->ReadBoolean();
	activate_next_subquest = br->ReadBoolean();
	on_give_up_parent_fails = br->ReadBoolean();
	on_success_parent_success = br->ReadBoolean();
	can_give_up = br->ReadBoolean();
	repeatable = br->ReadBoolean();
	repeatable_after_failure = br->ReadBoolean();
	fail_on_death = br->ReadBoolean();
	on_fail_parent_fail = br->ReadBoolean();
	UNKNOWN_10 = br->ReadBoolean();
	player_limit = br->ReadInt32();

	// this location is available in version 55+
	// but location structure has been updated in version 89+
	// required to read old format and new format
	trigger_locations = gcnew LocationSpan();
	if(version >= 89)
	{
		trigger_locations->has_location = br->ReadBoolean();
		trigger_locations->map_id = br->ReadInt32();
		trigger_locations->count = br->ReadInt32();
		trigger_locations->spans = gcnew array<Span^>(trigger_locations->count);
		trigger_locations->unknown_1 = br->ReadBytes(4);
	}
	else
	{
		trigger_locations->has_location = br->ReadBoolean();
		trigger_locations->map_id = br->ReadInt32();
		trigger_locations->count = 1;
		trigger_locations->spans = gcnew array<Span^>(trigger_locations->count);
		trigger_locations->spans[0] = ReadSpan(version, br);
		trigger_locations->unknown_1 = gcnew array<unsigned char>(4);
	}

	// this location is only available in version 89+
	fail_locations = gcnew LocationSpan();
	if(version >= 89)
	{
		fail_locations->has_location = br->ReadBoolean();
		fail_locations->map_id = br->ReadInt32();
		fail_locations->count = br->ReadInt32();
		fail_locations->spans = gcnew array<Span^>(fail_locations->count);
		fail_locations->unknown_1 = br->ReadBytes(4);
	}
	else
	{
		fail_locations->has_location = false;
		fail_locations->map_id = 0;
		fail_locations->count = 0;
		fail_locations->spans = gcnew array<Span^>(0);
		fail_locations->unknown_1 = gcnew array<unsigned char>(4);
	}

	// this location is only available in version 89+
	valid_locations = gcnew LocationSpan();
	if(version >= 89)
	{
		valid_locations->has_location = br->ReadBoolean();
		valid_locations->map_id = br->ReadInt32();
		valid_locations->count = br->ReadInt32();
		valid_locations->spans = gcnew array<Span^>(valid_locations->count);
		valid_locations->unknown_1 = br->ReadBytes(4);
	}
	else
	{
		valid_locations->has_location = false;
		valid_locations->map_id = 0;
		valid_locations->count = 0;
		valid_locations->spans = gcnew array<Span^>(0);
		valid_locations->unknown_1 = gcnew array<unsigned char>(4);
	}

	if(version >= 100)
	{
		UNKNOWN_17 = br->ReadBoolean();
	}
	else
	{
		UNKNOWN_17 = false;
	}

	has_instant_teleport = br->ReadBoolean();
	instant_teleport_location = ReadLocation(version, br);
	ai_trigger = br->ReadInt32();
	UNKNOWN_18 = br->ReadBoolean();
	UNKNOWN_19 = br->ReadBoolean();
	UNKNOWN_20 = br->ReadBoolean();

	if(version >= 89)
	{
		UNKNOWN_21 = br->ReadBoolean();
		UNKNOWN_22 = br->ReadBoolean();
	}
	else
	{
		UNKNOWN_21 = false;
		UNKNOWN_22 = false;
	}

	UNKNOWN_23 = br->ReadBoolean();
	UNKNOWN_LEVEL = br->ReadInt32();
	mark_available_icon = br->ReadBoolean();
	mark_available_point = br->ReadBoolean();
	quest_npc = br->ReadInt32();
	reward_npc = br->ReadInt32();
	craft_skill = br->ReadBoolean();
	UNKNOWN_24 = br->ReadBoolean();
	UNKNOWN_25 = br->ReadBoolean();
	UNKNOWN_26 = br->ReadBoolean();

	pq = gcnew PQ_Audit();
	if(version >= 89)
	{
		pq->script_info_count = br->ReadInt32();
		pq->unknown_1 = br->ReadBytes(12);
		pq->unknown_2 = br->ReadInt32();
		pq->unknown_3 = br->ReadBytes(15);
		pq->script_count = br->ReadInt32();
		pq->unknown_4 = br->ReadBytes(8);
		pq->unknown_5 = br->ReadBytes(1);
		pq->unknown_6 = br->ReadBytes(1);
		pq->chase_count = br->ReadInt32();
		pq->unknown_7 = br->ReadBytes(4);
		pq->required_quests_completed = br->ReadInt32();
		pq->unknown_8 = br->ReadBytes(1);

		// correct position not confirmed
		if(version >= 92)
		{
			pq->unknown_9 = br->ReadBytes(5);
		}
		else
		{
			pq->unknown_9 = gcnew array<unsigned char>(5);
		}

		pq->unknown_10 = br->ReadBytes(5);
	}
	else
	{
		pq->script_info_count = 0;
		pq->unknown_1 = gcnew array<unsigned char>(12);
		pq->unknown_2 = 0;
		pq->unknown_3 = gcnew array<unsigned char>(15);
		pq->script_count = 0;
		pq->unknown_4 = gcnew array<unsigned char>(8);
		pq->unknown_5 = gcnew array<unsigned char>(1);
		pq->unknown_6 = gcnew array<unsigned char>(1);
		pq->chase_count = 0;
		pq->unknown_7 = gcnew array<unsigned char>(4);
		pq->required_quests_completed = 0;
		pq->unknown_8 = gcnew array<unsigned char>(1);
		pq->unknown_9 = gcnew array<unsigned char>(5);
		pq->unknown_10 = gcnew array<unsigned char>(5);
	}
	pq->script_infos = gcnew array<PQ_AuditScriptInfo^>(pq->script_info_count);
	pq->scripts = gcnew array<PQ_AuditScript^>(pq->script_count);
	pq->chases = gcnew array<PQ_AuditChase^>(pq->chase_count);

	if(version >= 105)
	{
		LeaveFactionFail = br->ReadBoolean();
	}
	else
	{
		LeaveFactionFail = false;
	}
	if(version >= 108)
	{
		NotClearItemWhenFailed = br->ReadBoolean();
	}
	else
	{
		NotClearItemWhenFailed = false;
	}
	if(version >= 100)
	{
		UNKNOWN_26_01 = br->ReadBytes(2);
	}
	else
	{
		UNKNOWN_26_01 = gcnew array<unsigned char>(2);
	}

	level_min = br->ReadInt32();
	level_max = br->ReadInt32();
	UNKNOWN_27 = br->ReadBoolean();
	required_items_count = br->ReadInt32();
	required_items_unknown = br->ReadBytes(4);
	UNKNOWN_28 = br->ReadBoolean();

	if(version >= 100)
	{
		UNKNOWN_28_01 = br->ReadBoolean();
	}
	else
	{
		UNKNOWN_28_01 = false;
	}

	given_items_count = br->ReadInt32();
	UNKNOWN_29 = br->ReadInt32();
	UNKNOWN_30 = br->ReadInt32();
	given_items_unknown = br->ReadBytes(4);
	instant_pay_coins = br->ReadInt32();
	UNKNOWN_31 = br->ReadBoolean();
	required_reputation = br->ReadInt32();
	UNKNOWN_32 = br->ReadBytes(4);
	UNKNOWN_33 = br->ReadBoolean();
	required_quests_done_count = br->ReadInt32();
	required_quests_done = gcnew array<int>(5);
	for(int i=0; i<required_quests_done->Length; i++)
	{
		required_quests_done[i] = br->ReadInt32();
	}

	if(version >= 90)
	{
		UNKNOWN_34 = br->ReadBytes(60);
	}
	else
	{
		UNKNOWN_34 = gcnew array<unsigned char>(60);
	}

	UNKNOWN_35 = br->ReadBoolean();

	if(version >= 90)
	{
		UNKNOWN_36 = br->ReadInt32();
	}
	else
	{
		UNKNOWN_36 = 0;
	}

	required_cultivation = br->ReadInt32();
	UNKNOWN_37 = br->ReadBytes(5);

	if(version >= 93)
	{
		UNKNOWN_38 = br->ReadInt32();
	}
	else
	{
		UNKNOWN_38 = 0;
	}

	UNKNOWN_39 = br->ReadBoolean();

	required_gender = br->ReadInt32();
	UNKNOWN_40 = br->ReadBoolean();

	required_occupations = gcnew array<int>(10);

	required_occupations_count = br->ReadInt32();

	if(version >= 89)
	{
		for(int i=0; i<10; i++)
		{
			required_occupations[i] = br->ReadInt32();
		}
	}
	else
	{
		for(int i=0; i<8; i++)
		{
			required_occupations[i] = br->ReadInt32();
		}
	}

	UNKNOWN_41 = br->ReadBoolean();
	required_be_married = br->ReadBoolean();
	UNKNOWN_42 = br->ReadBoolean();

	if(version >= 102)
	{
		UNKNOWN_42_1 = br->ReadBoolean();
		UNKNOWN_42_2 = br->ReadBoolean();
	}
	else
	{
		UNKNOWN_42_1 = false;
		UNKNOWN_42_2 = true;
	}

	required_be_gm = br->ReadBoolean();
	UNKNOWN_43 = br->ReadBoolean();

	if(version >= 89)
	{
		UNKNOWN_44 = br->ReadBytes(19);
		date_unknown = ReadDate(version, br);
		UNKNOWN_45 = br->ReadInt32();
		UNKNOWN_46 = br->ReadBoolean();
	}
	else
	{
		UNKNOWN_44 = gcnew array<unsigned char>(19);
		date_unknown = gcnew Date();
		date_unknown->year = 0;
		date_unknown->month = 0;
		date_unknown->day = 0;
		date_unknown->weekday = 0;
		date_unknown->hour = 0;
		date_unknown->minute = 0;
		UNKNOWN_45 = 0;
		UNKNOWN_46 = false;
	}

	UNKNOWN_47 = br->ReadBytes(7);
	required_quests_undone_count = br->ReadInt32();
	required_quests_undone = gcnew array<int>(5);
	for(int i=0; i<required_quests_undone->Length; i++)
	{
		required_quests_undone[i] = br->ReadInt32();
	}
	required_blacksmith_level = br->ReadInt32();
	required_tailor_level = br->ReadInt32();
	required_craftsman_level = br->ReadInt32();
	required_apothecary_level = br->ReadInt32();
	UNKNOWN_48 = br->ReadBytes(32);

	if(version >= 89)
	{
		UNKNOWN_49 = br->ReadBytes(3);
	}
	else
	{
		UNKNOWN_49 = gcnew array<unsigned char>(3);
	}

	required_team_member_groups_count = br->ReadInt32();
	UNKNOWN_50 = br->ReadBytes(4);
	UNKNOWN_51 = br->ReadBoolean();

	if(version >= 89)
	{
		UNKNOWN_52 = br->ReadBytes(9);
		resource_pq_audit_id = br->ReadInt32();
		UNKNOWN_53 = br->ReadInt32();
		UNKNOWN_54 = br->ReadInt32();
		required_pq_contribution = br->ReadInt32();
		UNKNOWN_55 = br->ReadBytes(20);
	}
	else
	{
		UNKNOWN_52 = gcnew array<unsigned char>(9);
		resource_pq_audit_id = 0;
		UNKNOWN_53 = 0;
		UNKNOWN_54 = 0;
		required_pq_contribution = 0;
		UNKNOWN_55 = gcnew array<unsigned char>(20);
	}

	// need to verify correct position
	// between required_pq_contribution <-> required_success_type
	// inside unknown_55 ?
	if(version >= 100)
	{
		UNKNOWN_55_02_01 = br->ReadBytes(1);
		required_force = br->ReadInt32();
		UNKNOWN_55_02_02 = br->ReadBytes(1);
		required_prestige = br->ReadInt32();
		UNKNOWN_55_03 = br->ReadBytes(1);
		required_influence_fee = br->ReadInt32();
		UNKNOWN_55_04 = br->ReadBytes(11);
		UNKNOWN_55_05 = br->ReadBytes(4);
		UNKNOWN_55_06 = br->ReadBytes(1);
		if(version >= 108)
		{
			PremIsKing = br->ReadBoolean();
			ShowByKing = br->ReadBoolean();
			PremNotInTeam = br->ReadBoolean();
			ShowByNotInTeam = br->ReadBoolean();
		}
		else
		{
			PremIsKing = false;
			ShowByKing = false;
			PremNotInTeam = false;
			ShowByNotInTeam = false;
		}
	}
	else
	{
		UNKNOWN_55_02_01 = gcnew array<unsigned char>(1);
		required_force = 0;
		UNKNOWN_55_02_02 = gcnew array<unsigned char>(1);
		required_prestige = 0;
		UNKNOWN_55_03 = gcnew array<unsigned char>(1);
		required_influence_fee = 0;
		UNKNOWN_55_04 = gcnew array<unsigned char>(11);
		UNKNOWN_55_05 = gcnew array<unsigned char>(4);
		UNKNOWN_55_06 = gcnew array<unsigned char>(1);
	}

	required_success_type = br->ReadInt32();
	required_npc_type = br->ReadInt32();

	if(version >= 103)
	{
		required_morai_pk_count = br->ReadInt32();
		required_morai_pk_unknown = br->ReadBytes(4);
	}
	else
	{
		required_morai_pk_count = 0;
		required_morai_pk_unknown = gcnew array<unsigned char>(4);
	}

	required_chases_count = br->ReadInt32();
	required_chases_unknown = br->ReadBytes(4);
	required_get_items_count = br->ReadInt32();
	required_get_items_unknown = br->ReadBytes(4);
	required_coins = br->ReadInt32();
	UNKNOWN_56 = br->ReadBytes(16);

	if(version >= 89)
	{
		UNKNOWN_57 = br->ReadBytes(12);
	}
	else
	{
		UNKNOWN_57 = gcnew array<unsigned char>(12);
	}

	reach_locations = gcnew LocationSpan();
	if(version >= 89)
	{
		// reach_locations->has_location = br->ReadBoolean();
reach_locations->has_location = false;
		reach_locations->count = br->ReadInt32();
		reach_locations->spans = gcnew array<Span^>(reach_locations->count);
		reach_locations->map_id = br->ReadInt32();
		// reach_locations->unknown_1 = br->ReadBytes(4);
reach_locations->unknown_1 = gcnew array<unsigned char>(4);
	}
	else
	{
		reach_locations->has_location = false;
		reach_locations->count = 1;
		reach_locations->spans = gcnew array<Span^>(reach_locations->count);
		reach_locations->spans[0] = ReadSpan(version, br);
		reach_locations->map_id = br->ReadInt32();
		reach_locations->unknown_1 = gcnew array<unsigned char>(4);
	}

	required_wait_time = br->ReadInt32();

	if(version >= 100)
	{
		UNKNOWN_57_01 = br->ReadBytes(15);
	}
	else
	{
		UNKNOWN_57_01 = gcnew array<unsigned char>(15);
	}

	// At this place the v89 positions must be guessed due to leak of informations...

	pq->location = gcnew LocationSpan();

	if(version >= 89)
	{
		pq->unknown_11 = br->ReadBytes(4);
pq->location->has_location = false;
		pq->location->map_id = br->ReadInt32();
		pq->location->count = br->ReadInt32();
pq->location->unknown_1 = gcnew array<unsigned char>(4);
		pq->unknown_12 = br->ReadInt32();
		pq->unknown_13 = br->ReadBytes(5);
		pq->id_script = br->ReadInt32();
		pq->unknown_14 = br->ReadInt32();
		pq->unknown_15 = br->ReadInt32();
		pq->unknown_16 = br->ReadInt32();
		pq->unknown_17 = br->ReadBytes(20);
		pq->special_script_count = br->ReadInt32();
		pq->unknown_18 = br->ReadBytes(4);
		pq->unknown_19 = br->ReadBytes(4);
		pq->message_count = br->ReadInt32();
		pq->unknown_20 = br->ReadBytes(4);
	}
	else
	{
		pq->unknown_11 = gcnew array<unsigned char>(4);
pq->location->has_location = false;
		pq->location->map_id = 0;
		pq->location->count = 0;
pq->location->unknown_1 = gcnew array<unsigned char>(4);
		pq->unknown_12 = 0;
		pq->unknown_13 = gcnew array<unsigned char>(5);
		pq->id_script = 0;
		pq->unknown_14 = 0;
		pq->unknown_15 = 0;
		pq->unknown_16 = 0;
		pq->unknown_17 = gcnew array<unsigned char>(20);
		pq->special_script_count = 0;
		pq->unknown_18 = gcnew array<unsigned char>(4);
		pq->unknown_19 = gcnew array<unsigned char>(4);
		pq->message_count = 0;
		pq->unknown_20 = gcnew array<unsigned char>(4);
	}

	pq->location->spans = gcnew array<Span^>(pq->location->count);
	pq->special_scripts = gcnew array<PQ_AuditScript^>(pq->special_script_count);
	pq->messages = gcnew array<array<unsigned char>^>(pq->message_count);

	// Need to checked if contains something != 0
	UNKNOWN_58 = br->ReadBytes(8);
	if(version >= 105)
	{
		TransformedForm = br->ReadBytes(1);
	}
	else
	{
		TransformedForm = gcnew array<unsigned char>(1);
	}
	UNKNOWN_59 = br->ReadBytes(24);
	parent_quest = br->ReadInt32();
	previous_quest = br->ReadInt32();
	next_quest = br->ReadInt32();
	sub_quest_first = br->ReadInt32();

	if(version >= 89)
	{
		UNKNOWN_60 = br->ReadBoolean();
		receive_quest_probability = br->ReadSingle();
	}
	else
	{
		UNKNOWN_60 = false;
		receive_quest_probability = 0.0f;
	}

	if(version >= 100)
	{
		UNKNOWN_60_01 = br->ReadBoolean();
	}
	else
	{
		UNKNOWN_60_01 = false;
	}

// ################# AUTHOR TEXT #############################

	author_text = gcnew array<unsigned char>(0);
	if(author_mode)
	{
		author_text = br->ReadBytes(60);
	}

// ################# DATES #############################

	date_spans = gcnew array<DateSpan^>(date_spans_count);
	for(int i=0; i<date_spans->Length; i++)
	{
		date_spans[i] = ReadDateSpan(version, br);
	}

// ################# PQ AUDIT #############################

	if(version >= 89)
	{
		for(int i=0; i<pq->script_info_count; i++)
		{
			pq->script_infos[i] = gcnew PQ_AuditScriptInfo();
			pq->script_infos[i]->id = br->ReadInt32();
			pq->script_infos[i]->unknown_1 = br->ReadInt32();
			pq->script_infos[i]->unknown_2 = br->ReadBytes(1);
		}

		for(int i=0; i<pq->script_count; i++)
		{
			pq->scripts[i] = gcnew PQ_AuditScript();
			pq->scripts[i]->name = br->ReadBytes(64);
			pq->scripts[i]->count = br->ReadInt32();
			pq->scripts[i]->id = br->ReadSingle();
			pq->scripts[i]->seperator = br->ReadBytes(4);
			pq->scripts[i]->reference_id = br->ReadSingle();
			pq->scripts[i]->code = br->ReadBytes(496);
		}
		// exact arrangement of pq->chase cannot be determined
		// it would be guessed this data follows directly after pq->scripts
		for(int i=0; i<pq->chase_count; i++)
		{
			pq->chases[i] = gcnew PQ_AuditChase();
			pq->chases[i]->id_monster = br->ReadInt32();
			pq->chases[i]->amount_1 = br->ReadInt32();
			pq->chases[i]->contribution = br->ReadInt32();
			pq->chases[i]->amount_3 = br->ReadInt32();
		}
	}

// ################# LOCATIONS #############################
// before: 
// after: 
// unknown: 
	if(version >= 89)
	{
		for(int m=0; m<trigger_locations->count; m++)
		{
			trigger_locations->spans[m] = ReadSpan(version, br);
		}

		for(int m=0; m<fail_locations->count; m++)
		{
			fail_locations->spans[m] = ReadSpan(version, br);
		}

		for(int m=0; m<valid_locations->count; m++)
		{
			valid_locations->spans[m] = ReadSpan(version, br);
		}
	}

// ################# REQUIRED ITEMS #############################
// before: given_items
// after: valid_locaions
	required_items = gcnew array<Item^>(required_items_count);
	for(int i=0; i<required_items->Length; i++)
	{
		required_items[i] = ReadItem(version, br);
	}

// ################# GIVEN ITEMS #############################
// before: team_members
// after: required_items
	given_items = gcnew array<Item^>(given_items_count);
	for(int i=0; i<given_items->Length; i++)
	{
		given_items[i] = ReadItem(version, br);
	}

// ################# TEAM MEMBERS #############################
// after: given_items
// before: chase
	required_team_member_groups = gcnew array<TeamMembers^>(required_team_member_groups_count);
	for(int i=0; i<required_team_member_groups->Length; i++)
	{
		required_team_member_groups[i] = ReadTeamMembers(version, br);
	}

// ################# MORAI PK #############################
// after: ?
// before: ?
	required_morai_pk = gcnew array<MoraiPK^>(required_morai_pk_count);
	for(int i=0; i<required_morai_pk->Length; i++)
	{
		required_morai_pk[i] = ReadMoraiPK(version, br);
	}

// ################# CHASE #############################
// after: team_members
// before: pq->special_scripts
	required_chases = gcnew array<Chase^>(required_chases_count);
	for(int i=0; i<required_chases->Length; i++)
	{
		required_chases[i] = ReadChase(version, br);
	}

// ################# GET ITEMS #############################
// after: team_members
// before: pq->special_scripts
	required_get_items = gcnew array<Item^>(required_get_items_count);
	for(int i=0; i<required_get_items->Length; i++)
	{
		required_get_items[i] = ReadItem(version, br);
	}

// ################# LOCATIONS #############################
// after: team_members
// before: reward_success
	if(version >= 89)
	{
		for(int m=0; m<reach_locations->count;m++)
		{
			reach_locations->spans[m] = ReadSpan(version, br);
		}

		// exact arrangement of pq->leave_area cannot be determined
		// it would be guessed this data follows directly after reach_locations
		for(int m=0; m<pq->location->count; m++)
		{
			pq->location->spans[m] = ReadSpan(version, br);
		}
	}

// ################# PQ AUDIT #############################
// after: chases, get_items
// before: reward_success
	if(version >= 89)
	{
		for(int i=0; i<pq->special_script_count; i++)
		{
			pq->special_scripts[i] = gcnew PQ_AuditScript();
			pq->special_scripts[i]->name = br->ReadBytes(64);
			pq->special_scripts[i]->count = br->ReadInt32();
			pq->special_scripts[i]->id = br->ReadSingle();
			pq->special_scripts[i]->seperator = br->ReadBytes(4);
			pq->special_scripts[i]->reference_id = br->ReadSingle();
			pq->special_scripts[i]->code = br->ReadBytes(496);
		}

		for(int i=0; i<pq->message_count; i++)
		{
			pq->messages[i] = br->ReadBytes(128);
		}
	}

// ################# SUCCESS REWARDS #############################

	reward_success = ReadReward(version, br);

// ################# FAILED REWARDS #############################

	reward_failed = ReadReward(version, br);

// ################# TIMED REWARDS #############################

	rewards_timed_count = br->ReadInt32();
	rewards_timed_factors = gcnew array<float>(5);
	for(int i=0; i<rewards_timed_factors->Length; i++)
	{
		rewards_timed_factors[i] = br->ReadSingle();
	}
	rewards_timed = gcnew array<Reward^>(rewards_timed_count);
	for(int i=0; i<rewards_timed->Length; i++)
	{
		rewards_timed[i] = ReadReward(version, br);
	}

// ################# UNKNOWN REWARDS ??? #############################

	UNKNOWN_61 = br->ReadBytes(80);

// ################# CONVERSATION #############################

	conversation = gcnew Conversation();
	conversation->crypt_key = id;
	conversation->prompt_character_count = br->ReadInt32();
	conversation->prompt_text = br->ReadBytes(2*conversation->prompt_character_count);
	conversation->seperator = br->ReadBytes(8);
	conversation->general_character_count = br->ReadInt32();
	conversation->general_text = br->ReadBytes(2*conversation->general_character_count);
	conversation->dialogs = gcnew array<Dialog^>(5);
	for(int d=0; d<conversation->dialogs->Length; d++)
	{
		conversation->dialogs[d] = gcnew Dialog();
		conversation->dialogs[d]->crypt_key = id;
		conversation->dialogs[d]->unknown = br->ReadInt32();
		conversation->dialogs[d]->dialog_text = br->ReadBytes(128);
		conversation->dialogs[d]->question_count = br->ReadInt32();
		conversation->dialogs[d]->questions = gcnew array<Question^>(conversation->dialogs[d]->question_count);
		for(int q=0; q<conversation->dialogs[d]->questions->Length; q++)
		{
			conversation->dialogs[d]->questions[q] = gcnew Question();
			conversation->dialogs[d]->questions[q]->crypt_key = id;
			conversation->dialogs[d]->questions[q]->question_id = br->ReadInt32();
			conversation->dialogs[d]->questions[q]->previous_question = br->ReadInt32();
			conversation->dialogs[d]->questions[q]->question_character_count = br->ReadInt32();
			conversation->dialogs[d]->questions[q]->question_text = br->ReadBytes(2*conversation->dialogs[d]->questions[q]->question_character_count);
			conversation->dialogs[d]->questions[q]->answer_count = br->ReadInt32();
			conversation->dialogs[d]->questions[q]->answers = gcnew array<Answer^>(conversation->dialogs[d]->questions[q]->answer_count);
			for(int a=0; a<conversation->dialogs[d]->questions[q]->answer_count; a++)
			{
				conversation->dialogs[d]->questions[q]->answers[a] = gcnew Answer();
				conversation->dialogs[d]->questions[q]->answers[a]->crypt_key = id;
				conversation->dialogs[d]->questions[q]->answers[a]->question_link = br->ReadInt32();
				conversation->dialogs[d]->questions[q]->answers[a]->answer_text = br->ReadBytes(128);
				conversation->dialogs[d]->questions[q]->answers[a]->task_link = br->ReadInt32();
			}
		}
	}

// ################# ADD TREE-NODE #############################

	if(nodes)
	{
		Drawing::Color c = Drawing::Color::White;
		String^ node = Name;

		if(node->StartsWith("^"))
		{
			try
			{
				c = Drawing::Color::FromArgb(int::Parse(node->Substring(1, 6), Globalization::NumberStyles::HexNumber));
				node = node->Substring(7);
			}
			catch(...)
			{
				c = Drawing::Color::White;
			}
		}

		nodes->Add(id.ToString() + " - " + node);
		nodes[nodes->Count-1]->ForeColor = c;
	}

// ################# SUB TASKS #############################

	sub_quest_count = br->ReadInt32();
	sub_quests = gcnew array<Task^>(sub_quest_count);

	for(int i=0; i<sub_quest_count; i++)
	{
		if(nodes)
		{
			sub_quests[i] = gcnew Task(version, br, (int)br->BaseStream->Position, nodes[nodes->Count-1]->Nodes);
		}
		else
		{
			sub_quests[i] = gcnew Task(version, br, (int)br->BaseStream->Position, nullptr);
		}
	}
}