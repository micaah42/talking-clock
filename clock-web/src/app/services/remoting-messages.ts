export type RemotingMessageType = 'call' | 'get' | 'set' | 'subscribe' | 'notify' | 'return' | 'unsubscribe';

export interface RemotingMessage {
    type: RemotingMessageType;
    key: string;
}

export interface CallRemotingMessage extends RemotingMessage {
    type: 'call';
    args: any[];
}

export interface GetRemotingMessage extends RemotingMessage {
    type: 'get';
}

export interface SetRemotingMessage extends RemotingMessage {
    type: 'set';
    value: any;
}

export interface SubscribeRemotingMessage extends RemotingMessage {
    type: 'subscribe';
}
export interface UnsubscribeRemotingMessage extends RemotingMessage {
    type: 'unsubscribe';
}

export interface NotifyRemotingMessage extends RemotingMessage {
    type: 'notify';
    value: any;
}

export interface ReturnRemotingMessage extends RemotingMessage {
    type: 'return';
    value: any;
}