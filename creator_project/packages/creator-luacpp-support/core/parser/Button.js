const Node = require('./Node');
const Utils = require('../Utils');
const Sprite = require('./Sprite');
const state = require('./Global').state;
const get_sprite_frame_name_by_uuid = require('./Utils').get_sprite_frame_name_by_uuid;

class Button extends Node {
    constructor(data) {
        super(data);
        this._jsonNode.object_type = 'Button';
    }

    parse_properties() {
        super.parse_properties();

        this._properties = {node: this._properties};
        let spr_component = Node.get_node_component_of_type(this._node_data, 'cc.Sprite');
        let but_component = Node.get_node_component_of_type(this._node_data, 'cc.Button');

        this.add_property_int('duration', 'duration', but_component);

        this._properties.ignoreContentAdaptWithSize = false;

        // normal sprite: get from sprite component
        if (spr_component && spr_component._spriteFrame)
        {
            this._properties.spriteFrameName = get_sprite_frame_name_by_uuid(spr_component._spriteFrame.__uuid__);
            this._properties.trimEnabled = spr_component._isTrimmedMode
            this._properties.spriteType = Sprite.SPRITE_TYPES[spr_component._type];
        }
        else{
            let background_node_id = but_component._N$target.__id__;
            let background_node_data = state._json_data[background_node_id];
            let background_component_id = background_node_data._components[0].__id__;
            let background_component = state._json_data[background_component_id];
             if (background_component._spriteFrame)
            {
                this._properties.spriteFrameName = get_sprite_frame_name_by_uuid(background_component._spriteFrame.__uuid__);

                this._properties.backgroundNodeName = background_node_data._name
            }
        

        }
        
        // transition
        let transition = but_component.transition;
        this._properties.transition = transition;
        if (transition == 1) // COLOR transition
        {
            this.add_property_rgba('normalColor', '_N$normalColor', but_component);
            this.add_property_rgba('pressedColor', 'pressedColor', but_component);
            this.add_property_rgba('disableColor', '_N$disabledColor', but_component);
        }
           

       // if (transition == 3) // SCALE transition
        this._properties.zoomScale = but_component.zoomScale;
        
        if (transition == 2) {
            // SRPITE transition
            // pressed sprite
            if (but_component.pressedSprite)
                this._properties.pressedSpriteFrameName = get_sprite_frame_name_by_uuid(but_component.pressedSprite.__uuid__);
            // disabled sprite
            if (but_component._N$disabledSprite)
                this._properties.disabledSpriteFrameName = get_sprite_frame_name_by_uuid(but_component._N$disabledSprite.__uuid__);
        }
    }
}

module.exports = Button;