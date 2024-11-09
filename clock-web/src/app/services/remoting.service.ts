import { Injectable } from '@angular/core';
import { BehaviorSubject, Observable, Subject, filter } from 'rxjs';
import { WebSocketSubject, webSocket } from 'rxjs/webSocket';

import { NotifyRemotingMessage, RemotingMessage, SubscribeRemotingMessage } from './remoting-messages';

type State = 'disconnected' | 'connecting' | 'connected'

@Injectable({
  providedIn: 'root'
})
export class RemotingService {
  private subjects: Map<string, Subject<any>> = new Map<string, Subject<any>>()
  private observables: Map<string, Observable<any>> = new Map<string, Observable<any>>()
  private websocket$: WebSocketSubject<RemotingMessage>;
  private state$: BehaviorSubject<State> = new BehaviorSubject<State>('connecting')


  constructor() {
    this.websocket$ = webSocket({
      openObserver: {
        next: (event) => {
          console.log('open event', event)
          this.state$.next('connected')
        }
      },
      closeObserver: {
        next: (reason) => {
          console.warn('ws open error:', reason)
          this.state$.next('disconnected')
        }
      },
      protocol: 'ws',
      url: '/ws'
    })

    this.websocket$.subscribe({
      next: (message) => this.handleMessage(message),
      error: (error) => console.warn('received error:', error),
      complete: () => console.log('socket complete:')
    })

    this.state$.pipe(filter(x => x === 'connected')).subscribe(() => {
      // 'on connected'
      
      for (let key of this.observables.keys())
        this.sendMessage({ type: 'subscribe', key })
    })
  }

  private handleMessage(message: RemotingMessage) {
    console.log('received message:', message)

    switch (message.type) {
      case 'notify':
        if (this.subjects.has(message.key))
          this.subjects.get(message.key)!.next((message as NotifyRemotingMessage).value);
        return;

      case 'return':
      default:
        console.warn('whoops')
    }

  }

  private sendMessage(message: RemotingMessage) {
    if (this.state$.value === 'connected')
      return this.websocket$.next(message)
  }

  subscribe<T>(key: string): Observable<T> {

    if (!this.observables.has(key)) {
      const newSubject = new Subject<T>()
      this.subjects.set(key, newSubject)
      this.observables.set(key, newSubject.asObservable())
      this.sendMessage({ type: 'subscribe', key })
    }

    return this.observables.get(key)!;
  }


}
