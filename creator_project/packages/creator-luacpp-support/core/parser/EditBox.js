const Node = require('./Node');
const Utils = require('./Utils');
const state = require('./Global').state;

class EditBox extends Node {
    constructor(data) {
        super(data);
        this._jsonNode.object_type = 'EditBox';
    }

    parse_properties() {
        super.parse_properties();

        this._properties = {node: this._properties};

        let component = Node.get_node_component_of_type(this._node_data, 'cc.EditBox');

        // background image is needed by cocos2d-x's EditBox
        if (!component._N$backgroundImage){
           // Utils.log("Error:EditBox background image is needed by cocos2d-x!");
           this.initBackground(component);
           this.initTextLabel(component);
           this.initPlaceholderTextLabel(component);
        }
        else
        {
            this._properties.backgroundImage = Utils.get_sprite_frame_name_by_uuid(component._N$backgroundImage.__uuid__);

        }

        //this._properties.backgroundImage = Utils.get_sprite_frame_name_by_uuid(component._N$backgroundImage.__uuid__);

        

        this._properties.returnType = EditBox.RETURN_TYPE[component._N$returnType];
        this._properties.inputFlag = EditBox.INPUT_FLAG[component._N$inputFlag];
        this._properties.inputMode = EditBox.INPUT_MODE[component._N$inputMode];
        this.add_property_int('fontSize', '_N$fontSize', component);
        this.add_property_rgb('fontColor', '_N$fontColor', component);
        this.add_property_str('placeholder', '_N$placeholder', component);
        this.add_property_int('placeholderFontSize', '_N$placeholderFontSize', component);
        this.add_property_rgb('placeholderFontColor', '_N$placeholderFontColor', component);
        this.add_property_int('maxLength', '_N$maxLength', component);
        this.add_property_str('text', '_string', component);

    }


     initTextLabel(editbox_component) {
        let pageview_node_id = editbox_component._N$textLabel.__id__;
        let pageview_node = state._json_data[pageview_node_id];

        let background_id = pageview_node.node.__id__;
        let background_node = state._json_data[background_id];
        let background_label_component = Node.get_node_component_of_type(background_node, 'cc.Label');
        let node = state._json_data[background_label_component.node.__id__];

        this.add_property_int('fontSize', '_fontSize', pageview_node);
        this.add_property_rgb('fontColor', '_color', node);
        Utils.remove_child_by_id(this, background_id);
       
    }

    initPlaceholderTextLabel(editbox_component) {
        let pageview_node_id = editbox_component._N$placeholderLabel.__id__;
        let pageview_node = state._json_data[pageview_node_id];
        let background_id = pageview_node.node.__id__;
        let background_node = state._json_data[background_id];

        let background_label_component = Node.get_node_component_of_type(background_node, 'cc.Label');
        let node = state._json_data[background_label_component.node.__id__];

        this.add_property_int('placeholderFontSize', '_fontSize', pageview_node);
        this.add_property_rgb('placeholderFontColor', '_color', node);
        this.add_property_str('placeholder', '_string', pageview_node);
       Utils.remove_child_by_id(this, background_id);
    }


     initBackground(editbox_component) {
        let pageview_node_id = editbox_component.node.__id__;
        let pageview_node = state._json_data[pageview_node_id];
        if(pageview_node._children.length>0){
                    let background_id = pageview_node._children[0].__id__;
        let background_node = state._json_data[background_id];
        let background_sprite_component = Node.get_node_component_of_type(background_node, 'cc.Sprite');
        if (background_sprite_component._spriteFrame) {
            let background = {};
            let uuid = background_sprite_component._spriteFrame.__uuid__;
            background.spriteFrame = Utils.get_sprite_frame_name_by_uuid(uuid);
            background.spriteFrameFromTP = Utils.is_sprite_frame_from_texture_packer(uuid);
            if (!background.spriteFrameFromTP)
                background.spriteFrame = background.spriteFrame;

            //this._properties.background = background;
             this._properties.backgroundImage = background.spriteFrame
            }
             Utils.remove_child_by_id(this, background_id);
            return background_id;
        }

        return 0;
    }

}
EditBox.INPUT_MODE = ['Any', 'EmailAddress', 'Numeric', 'PhoneNumber', 'URL', 'Decime', 'SingleLine'];
EditBox.INPUT_FLAG = ['Password', 'Sensitive', 'InitialCapsWord', 'InitialCapsSentence', 'InitialCapsAllCharacters', 'LowercaseAllCharacters'];
EditBox.RETURN_TYPE = ['Default', 'Done', 'Send', 'Search', 'Go'];

module.exports = EditBox;