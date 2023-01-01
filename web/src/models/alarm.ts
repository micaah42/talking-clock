export interface Alarm {
  name: string,
  activated: boolean,
  repeatRule: boolean[],
  sound: string,
  time: string,
  id: number
}
