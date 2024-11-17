import { Injectable } from '@angular/core';
import { BehaviorSubject, Observable, ReplaySubject, filter } from 'rxjs';
import { WebSocketSubject, webSocket } from 'rxjs/webSocket';

import { NotifyRemotingMessage, RemotingMessage, SubscribeRemotingMessage } from './remoting-messages';

type State = 'disconnected' | 'connecting' | 'connected'

class SubscriptionSubject extends ReplaySubject<any> {
  public counter: number = 1;
}

@Injectable({
  providedIn: 'root'
})
export class RemotingService {
  private subjects: Map<string, SubscriptionSubject> = new Map<string, SubscriptionSubject>()
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

      for (let key of this.subjects.keys())
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

  subscribe<T>(key: string, destroy$: Observable<void>): Observable<T> {

    if (!this.subjects.has(key)) {
      const newSubject = new SubscriptionSubject()
      this.subjects.set(key, newSubject)
      this.sendMessage({ type: 'subscribe', key })
    }

    destroy$.subscribe(() => this.unsubscribe(key))

    const subject = this.subjects.get(key)!;
    subject.counter += 1;
    return subject.asObservable();
  }

  unsubscribe<T>(key: string) {
    if (!this.subjects.has(key))
      return

    const subject = this.subjects.get(key)!;
    subject.counter -= 1;

    if (subject.counter > 0)
      return;

      this.sendMessage({type: 'unsubscribe', key})

  }


}
